import os
import random
from typing import List

import numpy as np
import pandas as pd
import torch
import torch.nn as nn
import torch.nn.functional as F
from sklearn.metrics import roc_auc_score, accuracy_score
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
from torch.utils.data import DataLoader, TensorDataset

from utils import plot_training_curves, plot_roc_curve, plot_predictions_distribution

# Limit CPU threads
NUM_CPU = 12
os.environ["OMP_NUM_THREADS"] = str(NUM_CPU)
os.environ["MKL_NUM_THREADS"] = str(NUM_CPU)
torch.set_num_threads(NUM_CPU)
try:
    torch.set_num_interop_threads(NUM_CPU)
except Exception:
    pass

def set_seed(seed: int) -> None:
    random.seed(seed)
    np.random.seed(seed)
    torch.manual_seed(seed)
    torch.cuda.manual_seed_all(seed)
    torch.backends.cudnn.deterministic = True
    torch.backends.cudnn.benchmark = False

# Inputs
BASE_DIR = os.path.dirname(__file__)
TRAINING_PICKLE = os.path.abspath(f"{BASE_DIR}/from_albert/training_file_kaon_pion.pkl")
OUTPUT_DIR = os.path.abspath(f"{BASE_DIR}/outputs")
OUTPUT_PICKLE = f"{OUTPUT_DIR}/predictions.pkl"
CHECKPOINT_PATH = f"{OUTPUT_DIR}/best_model.pt"

# Configuration flag: True for tanh+MSE, False for ReLU+sigmoid+BCE
USE_TANH_MSE = True
USE_STANDARD_SCALER = True

# Training
TEST_SIZE = 0.3
RANDOM_STATE = 42
BATCH_SIZE = 1024
EPOCHS = 1000
EXTRA_HIDDEN_SIZE = 5
LR = 1e-3
WEIGHT_DECAY = 0
VALIDATE_EVERY = 25
FEATURE_COLS = [
    "Positive_Energy_FCAL",
    "Positive_E1E9_FCAL",
    "Positive_E9E25_FCAL",
    "Positive_SumU_FCAL",
    "Positive_SumV_FCAL",
    "Positive_TrackFCAL_DOCA",
    # --
    "Negative_Energy_FCAL",
    "Negative_E1E9_FCAL",
    "Negative_E9E25_FCAL",
    "Negative_SumU_FCAL",
    "Negative_SumV_FCAL",
    "Negative_TrackFCAL_DOCA",
]

# Optional TMVA-like cuts (commented; branches may not exist)
# df_sig = df_sig.query("8.2 < BeamP4_meas_E < 8.8")
# df_sig = df_sig.query("2 < sqrt(PositiveP4_kin_X**2 + PositiveP4_kin_Y**2 + PositiveP4_kin_Z**2) < 6")
# df_sig = df_sig.query("2 < sqrt(NegativeP4_kin_X**2 + NegativeP4_kin_Y**2 + NegativeP4_kin_Z**2) < 6")
# df_bkg = df_bkg.query("8.2 < BeamP4_meas_E < 8.8")
# df_bkg = df_bkg.query("2 < sqrt(PositiveP4_kin_X**2 + PositiveP4_kin_Y**2 + PositiveP4_kin_Z**2) < 6")
# df_bkg = df_bkg.query("2 < sqrt(NegativeP4_kin_X**2 + NegativeP4_kin_Y**2 + NegativeP4_kin_Z**2) < 6")

class MLP(nn.Module):
    def __init__(self, num_inputs: int):
        super().__init__()
        hidden_size = num_inputs + EXTRA_HIDDEN_SIZE
        self.fc1 = nn.Linear(num_inputs, hidden_size)
        self.fc_out = nn.Linear(hidden_size, 1)

    def forward(self, x: torch.Tensor) -> torch.Tensor:
        if USE_TANH_MSE:
            x = torch.tanh(self.fc1(x))
            x = torch.tanh(self.fc_out(x))
        else:
            x = F.relu(self.fc1(x))
            x = torch.sigmoid(self.fc_out(x))
        return x

def train_model() -> None:

    set_seed(RANDOM_STATE)
    
    # torch requries (data, model) to be on the same device
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")

    df = pd.read_pickle(TRAINING_PICKLE)
    
    # Set labels based on configuration
    # Class 1: K⁺K⁻
    # Class 0: π⁺π⁻
    if USE_TANH_MSE:
        # Since original network uses tanh, we need to output onto [-1, 1] range
        df["label"] = df["pi_k"].map({1: 1.0, 0: -1.0})
    else:
        # Use binary labels in {0, 1} for BCE
        df["label"] = df["pi_k"].map({1: 1.0, 0: 0.0})
    X = df[FEATURE_COLS].to_numpy(dtype=np.float32)
    y = df["label"].to_numpy(dtype=np.float32)
    
    # DOCA apparently takes on a value of 999 for some events, perhaps this means a tracking issue?
    #    What if we set it to -1 since most of the values are 1 +- 1
    df.loc[df['Positive_TrackFCAL_DOCA'] == 999, 'Positive_TrackFCAL_DOCA'] = -1
    df.loc[df['Negative_TrackFCAL_DOCA'] == 999, 'Negative_TrackFCAL_DOCA'] = -1

    # Split into training and test datasets
    X_train, X_test, y_train, y_test = train_test_split(
        X, y, test_size=TEST_SIZE, random_state=RANDOM_STATE, shuffle=True
    )
    
    # fit the scaler to training data only
    if USE_STANDARD_SCALER:
        scaler = StandardScaler()
        X_train = scaler.fit_transform(X_train)
        X_test = scaler.transform(X_test)
        X = scaler.transform(X)

    # DataLoaders
    train_loader = DataLoader(
        TensorDataset(torch.from_numpy(X_train).float(), torch.from_numpy(y_train).float().view(-1, 1)),
        batch_size=BATCH_SIZE,
        shuffle=True,
    )
    val_loader = DataLoader(
        TensorDataset(torch.from_numpy(X_test).float(), torch.from_numpy(y_test).float().view(-1, 1)),
        batch_size=BATCH_SIZE,
        shuffle=False,
    )

    # Model and optimizer
    model = MLP(num_inputs=X.shape[1]).to(device)
    optimizer = torch.optim.Adam(model.parameters(), lr=LR, weight_decay=WEIGHT_DECAY)
    criterion = nn.MSELoss() if USE_TANH_MSE else nn.BCELoss()

    # Track training history
    train_losses, val_losses = [], []
    val_aucs = []
    val_accs = []

    # Train
    best_val_auc = -np.inf
    os.makedirs(OUTPUT_DIR, exist_ok=True)
    for epoch in range(1, EPOCHS + 1):
        
        # Set model to training mode, some layers have different behavior in training and inference
        #   Not true with this model, but good in practice
        model.train()
        
        running_loss, num_samples = 0.0, 0
        for xb, yb in train_loader:
            xb = xb.to(device)
            yb = yb.to(device)
            
            optimizer.zero_grad() # torch accumulates gradients between iterations by default
            
            preds = model(xb)
            loss = criterion(preds, yb)
            
            loss.backward()  # compute gradients
            optimizer.step() # update weights
            
            running_loss += loss.item() * xb.size(0)
            num_samples += xb.size(0)
        avg_train_loss = running_loss / max(1, num_samples)
        train_losses.append(avg_train_loss)
        # Ensure validation arrays align with epochs regardless of validation cadence
        val_losses.append(np.nan)
        val_aucs.append(np.nan)
        val_accs.append(np.nan)

        # Track / log validation metrics intermittently
        if epoch % VALIDATE_EVERY == 0 or epoch == EPOCHS:
            model.eval() # set model to evaluation mode
            with torch.no_grad(): # no need to spend extra compute on gradient computation
                y_true_list, y_pred_list = [], []
                val_loss_sum, val_n = 0.0, 0
                for xb, yb in val_loader:
                    xb = xb.to(device)
                    yb = yb.to(device)
                    preds = model(xb)
                    loss = criterion(preds, yb)
                    val_loss_sum += loss.item() * xb.size(0)
                    val_n += xb.size(0)
                    y_true_list.append(yb.squeeze(1).cpu().numpy())
                    y_pred_list.append(preds.squeeze(1).cpu().numpy())
                y_true = np.concatenate(y_true_list, axis=0)
                y_pred = np.concatenate(y_pred_list, axis=0)
                
                if USE_TANH_MSE:
                    y_prob = (y_pred + 1.0) / 2.0
                    y_prob_true = ((y_true + 1.0) / 2.0).astype(int)
                    val_acc = accuracy_score(y_prob_true, (y_pred >= 0.0).astype(int))
                else:
                    y_prob = y_pred
                    y_prob_true = y_true.astype(int)
                    val_acc = accuracy_score(y_prob_true, (y_pred >= 0.5).astype(int))
                
                try:
                    val_auc = roc_auc_score(y_prob_true, y_prob)
                except ValueError:
                    val_auc = np.nan
                val_loss = val_loss_sum / max(1, val_n)
                
                # Store validation metrics for this epoch (overwrite placeholders)
                val_losses[-1] = val_loss
                val_aucs[-1] = val_auc
                val_accs[-1] = val_acc

            if val_auc > best_val_auc:
                best_val_auc = val_auc
                torch.save({"model_state": model.state_dict()}, CHECKPOINT_PATH)

            print(
                f"Epoch {epoch:4d} | train_loss={avg_train_loss:.5f} | val_loss={val_loss:.5f} | val_auc={val_auc:.4f} | val_acc={val_acc:.4f}"
            )

    # Plot training curves
    plot_training_curves(
        train_losses=train_losses,
        val_losses=val_losses,
        val_aucs=val_aucs,
        val_accs=val_accs,
        save_path=f"{OUTPUT_DIR}/training_curves.png"
    )

    # Load best and run inference on full set
    if os.path.exists(CHECKPOINT_PATH):
        state = torch.load(CHECKPOINT_PATH, map_location=device)
        model.load_state_dict(state["model_state"])
    model.eval()
    with torch.no_grad():
        feats_tensor = torch.from_numpy(X).float().to(device)
        preds = model(feats_tensor).squeeze(1).cpu().numpy()
        
        if USE_TANH_MSE:
            probs = (preds + 1.0) / 2.0
        else:
            probs = preds
    
    # Plot ROC curve
    if USE_TANH_MSE:
        y_true_binary = ((y + 1.0) / 2.0).astype(int)
    else:
        y_true_binary = y.astype(int)
    plot_roc_curve(
        y_true=y_true_binary,
        y_scores=probs,
        save_path=f"{OUTPUT_DIR}/roc_curve.png"
    )
    
    # Plot predictions distribution
    plot_predictions_distribution(
        y_true=y_true_binary,
        y_pred=probs,
        save_path=f"{OUTPUT_DIR}/predictions_distribution.png"
    )
    
    pred_series = pd.Series(probs, index=df.index, name="prob_signal")
    out_path = os.path.join(OUTPUT_DIR, OUTPUT_PICKLE)
    pred_series.to_pickle(out_path)
    print(f"Saved predictions to {out_path}")


if __name__ == "__main__":
    train_model()
