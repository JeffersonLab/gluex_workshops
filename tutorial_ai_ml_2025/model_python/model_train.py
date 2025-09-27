import os
import random
from typing import List, Tuple, Dict, Any, Optional

import numpy as np
import pandas as pd
import torch
import torch.nn as nn
from sklearn.metrics import roc_auc_score, accuracy_score
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
from torch.utils.data import DataLoader, TensorDataset
import optuna
from optuna.samplers import TPESampler
from optuna.storages import RDBStorage
import optuna_dashboard

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

# Paths and configuration
BASE_DIR = os.path.dirname(__file__)
TRAINING_PICKLE = os.path.abspath(f"{BASE_DIR}/from_albert/training_file_kaon_pion.pkl")
OUTPUT_DIR = os.path.abspath(f"{BASE_DIR}/outputs")
OUTPUT_PICKLE = f"{OUTPUT_DIR}/predictions.pkl"
CHECKPOINT_PATH = f"{OUTPUT_DIR}/best_model.pt"
OPTUNA_DB_URL = f"sqlite:///{OUTPUT_DIR}/optuna_studies.db"
STUDY_NAME = "mlp_hyperparameter_optimization"
os.makedirs(OUTPUT_DIR, exist_ok=True)

# Configuration flags
USE_STANDARD_SCALER = True

# Fixed hyperparameters
TEST_SIZE = 0.3
RANDOM_STATE = 42
EPOCHS = 1000
VALIDATE_EVERY = 10
FEATURE_COLS = [
    "Positive_Energy_FCAL",
    "Positive_E1E9_FCAL",
    "Positive_E9E25_FCAL",
    "Positive_SumU_FCAL",
    "Positive_SumV_FCAL",
    "Positive_TrackFCAL_DOCA",
    "Negative_Energy_FCAL",
    "Negative_E1E9_FCAL",
    "Negative_E9E25_FCAL",
    "Negative_SumU_FCAL",
    "Negative_SumV_FCAL",
    "Negative_TrackFCAL_DOCA",
]

class MLP(nn.Module):
    def __init__(self, num_inputs: int, hidden_sizes: List[int]):
        super().__init__()
        self.layers = nn.ModuleList()
        prev_size = num_inputs
        for hidden_size in hidden_sizes:
            self.layers.append(nn.Linear(prev_size, hidden_size))
            prev_size = hidden_size
        self.output_layer = nn.Linear(prev_size, 1)

    def forward(self, x: torch.Tensor) -> torch.Tensor:
        for layer in self.layers:
            x = torch.tanh(layer(x))
        x = torch.tanh(self.output_layer(x))
        return x

def suggest_hyperparameters(trial: optuna.Trial, num_inputs: int) -> Dict[str, Any]:
    """Suggest hyperparameters for a trial using Optuna's suggest methods."""
    return {
        'batch_size': trial.suggest_categorical('batch_size', [256, 512, 1024, 2048]),
        'learning_rate': trial.suggest_float('learning_rate', 1e-5, 1e-2, log=True),
        'weight_decay': trial.suggest_float('weight_decay', 1e-6, 1e-2, log=True),
        'hidden_layers': trial.suggest_int('hidden_layers', 1, 3),
        'hidden_size_multiplier': trial.suggest_float('hidden_size_multiplier', 0.5, 3.0),
    }

def create_model_architecture(params: Dict[str, Any], num_inputs: int) -> MLP:
    """Create MLP model based on suggested hyperparameters."""
    # Calculate hidden layer sizes
    hidden_sizes = []
    current_size = num_inputs
    
    for _ in range(params['hidden_layers']):
        layer_size = int(current_size * params['hidden_size_multiplier'])
        hidden_sizes.append(layer_size)
        current_size = layer_size  # For next layer
    
    return MLP(
        num_inputs=num_inputs,
        hidden_sizes=hidden_sizes
    )

def train_model(model: MLP, train_loader: DataLoader, val_loader: DataLoader, 
                device: torch.device, epochs: int = EPOCHS, 
                validate_every: int = VALIDATE_EVERY, patience: int = 50,
                return_history: bool = False) -> Tuple[float, float, Optional[Dict]]:
    """Train a model and return validation metrics."""
    optimizer = torch.optim.Adam(model.parameters())
    criterion = nn.MSELoss()
    
    # Track training progress
    train_losses, val_losses = [], []
    val_aucs, val_accs = [], []
    best_val_auc, best_val_acc = -np.inf, 0.0
    patience_counter = 0
    
    for epoch in range(1, epochs + 1):
        # Training
        model.train()
        running_loss = 0.0
        num_samples = 0
        
        for xb, yb in train_loader:
            xb, yb = xb.to(device), yb.to(device)
            optimizer.zero_grad()
            preds = model(xb)
            loss = criterion(preds, yb)
            loss.backward()
            optimizer.step()
            
            running_loss += loss.item() * xb.size(0)
            num_samples += xb.size(0)
        
        avg_train_loss = running_loss / max(1, num_samples)
        train_losses.append(avg_train_loss)
        val_losses.append(np.nan)
        val_aucs.append(np.nan)
        val_accs.append(np.nan)
        
        # Validation
        if epoch % validate_every == 0 or epoch == epochs:
            model.eval()
            with torch.no_grad():
                y_true_list, y_pred_list = [], []
                val_loss_sum, val_n = 0.0, 0
                
                for xb, yb in val_loader:
                    xb, yb = xb.to(device), yb.to(device)
                    preds = model(xb)
                    loss = criterion(preds, yb)
                    val_loss_sum += loss.item() * xb.size(0)
                    val_n += xb.size(0)
                    y_true_list.append(yb.squeeze(1).cpu().numpy())
                    y_pred_list.append(preds.squeeze(1).cpu().numpy())
                
                y_true = np.concatenate(y_true_list, axis=0)
                y_pred = np.concatenate(y_pred_list, axis=0)
                
                # Convert tanh output [-1,1] to probabilities [0,1]
                y_prob = (y_pred + 1.0) / 2.0
                y_prob_true = ((y_true + 1.0) / 2.0).astype(int)
                
                # Calculate metrics
                try:
                    val_auc = roc_auc_score(y_prob_true, y_prob)
                except ValueError:
                    val_auc = 0.5
                val_acc = accuracy_score(y_prob_true, (y_pred >= 0.0).astype(int))
                val_loss = val_loss_sum / max(1, val_n)
                
                val_losses[-1] = val_loss
                val_aucs[-1] = val_auc
                val_accs[-1] = val_acc
                
                # Track best metrics and early stopping
                if val_auc > best_val_auc:
                    best_val_auc, best_val_acc = val_auc, val_acc
                    patience_counter = 0
                else:
                    patience_counter += 1
                    if patience_counter >= patience:
                        break
    
    if return_history:
        history = {
            'train_losses': train_losses,
            'val_losses': val_losses,
            'val_aucs': val_aucs,
            'val_accs': val_accs
        }
        return best_val_auc, best_val_acc, history
    else:
        return best_val_auc, best_val_acc

def train_single_trial(trial: optuna.Trial, X_train: np.ndarray, y_train: np.ndarray, 
                      X_val: np.ndarray, y_val: np.ndarray, device: torch.device) -> Tuple[float, float]:
    """Train a single model trial and return validation metrics."""
    params = suggest_hyperparameters(trial, X_train.shape[1])
    
    # Track model architecture
    trial.set_user_attr("model_architecture", f"hidden_layers={params['hidden_layers']}, multiplier={params['hidden_size_multiplier']:.2f}")
    
    # Create model and track parameters
    model = create_model_architecture(params, X_train.shape[1]).to(device)
    total_params = sum(p.numel() for p in model.parameters())
    trial.set_user_attr("total_parameters", total_params)
    
    # Create data loaders
    train_loader = DataLoader(
        TensorDataset(torch.from_numpy(X_train).float(), torch.from_numpy(y_train).float().view(-1, 1)),
        batch_size=params['batch_size'],
        shuffle=True,
    )
    val_loader = DataLoader(
        TensorDataset(torch.from_numpy(X_val).float(), torch.from_numpy(y_val).float().view(-1, 1)),
        batch_size=params['batch_size'],
        shuffle=False,
    )
    
    # Train model
    best_val_auc, best_val_acc = train_model(
        model=model,
        train_loader=train_loader,
        val_loader=val_loader,
        device=device,
        epochs=EPOCHS,
        validate_every=VALIDATE_EVERY,
        patience=50
    )
    
    # Store results
    trial.set_user_attr("best_val_auc", best_val_auc)
    trial.set_user_attr("best_val_acc", best_val_acc)
    return best_val_auc, best_val_acc

def optimize_hyperparameters(n_trials: int = 100) -> Tuple[optuna.Study, np.ndarray, np.ndarray, np.ndarray, np.ndarray, StandardScaler]:
    """Run hyperparameter optimization using Optuna."""
    set_seed(RANDOM_STATE)
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    
    # Load and prepare data
    df = pd.read_pickle(TRAINING_PICKLE)
    df["label"] = df["pi_k"].map({1: 1.0, 0: -1.0})  # tanh output range [-1, 1]
    
    # Handle missing DOCA values
    df.loc[df['Positive_TrackFCAL_DOCA'] == 999, 'Positive_TrackFCAL_DOCA'] = -1
    df.loc[df['Negative_TrackFCAL_DOCA'] == 999, 'Negative_TrackFCAL_DOCA'] = -1
    
    X = df[FEATURE_COLS].to_numpy(dtype=np.float32)
    y = df["label"].to_numpy(dtype=np.float32)
    
    # Split data: train/val for optimization, test for final evaluation
    X_train, X_test, y_train, y_test = train_test_split(
        X, y, test_size=TEST_SIZE, random_state=RANDOM_STATE, shuffle=True
    )
    X_train, X_val, y_train, y_val = train_test_split(
        X_train, y_train, test_size=0.2, random_state=RANDOM_STATE, shuffle=True
    )
    
    # Apply scaling if requested
    scaler = None
    if USE_STANDARD_SCALER:
        scaler = StandardScaler()
        X_train = scaler.fit_transform(X_train)
        X_val = scaler.transform(X_val)
        X_test = scaler.transform(X_test)
        X = scaler.transform(X)
    
    # Create study and run optimization
    storage = RDBStorage(OPTUNA_DB_URL)
    try:
        study = optuna.load_study(
            study_name=STUDY_NAME,
            storage=storage,
            sampler=TPESampler(seed=RANDOM_STATE)
        )
        print(f"Loaded existing study: {STUDY_NAME} ({len(study.trials)} trials)")
    except KeyError:
        study = optuna.create_study(
            study_name=STUDY_NAME,
            storage=storage,
            directions=['maximize', 'maximize'],  # Multi-objective: AUC and accuracy
            sampler=TPESampler(seed=RANDOM_STATE)
        )
        print(f"Created new study: {STUDY_NAME}")

    def objective(trial):
        try:
            return train_single_trial(trial, X_train, y_train, X_val, y_val, device)
        except Exception as e:
            print(f"Trial failed: {e}")
            return 0.0, 0.0
    
    print(f"Starting optimization with {n_trials} trials...")
    study.optimize(objective, n_trials=n_trials)
    return study, X, y, X_test, y_test, scaler


def train_best_model(study: optuna.Study, X: np.ndarray, y: np.ndarray, 
                    X_test: np.ndarray, y_test: np.ndarray, scaler: StandardScaler = None) -> None:
    """Train the best model found by Optuna and generate final results."""
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    
    # Get best trial (highest AUC)
    best_trial = max(study.trials, key=lambda t: t.values[0] if t.values else -np.inf)
    best_params = best_trial.params
    
    print(f"Best trial: {best_trial.number}")
    print(f"Best AUC: {best_trial.values[0]:.4f}")
    print(f"Best Accuracy: {best_trial.values[1]:.4f}")
    print(f"Model architecture: {best_trial.user_attrs.get('model_architecture', 'N/A')}")
    print(f"Total parameters: {best_trial.user_attrs.get('total_parameters', 'N/A')}")
    
    # Create and train best model
    model = create_model_architecture(best_params, X.shape[1]).to(device)
    
    # Create data loaders
    train_loader = DataLoader(
        TensorDataset(torch.from_numpy(X).float(), torch.from_numpy(y).float().view(-1, 1)),
        batch_size=best_params['batch_size'],
        shuffle=True,
    )
    test_loader = DataLoader(
        TensorDataset(torch.from_numpy(X_test).float(), torch.from_numpy(y_test).float().view(-1, 1)),
        batch_size=best_params['batch_size'],
        shuffle=False,
    )
    
    # Train model with history tracking
    best_val_auc, best_val_acc, history = train_model(
        model=model,
        train_loader=train_loader,
        val_loader=test_loader,
        device=device,
        epochs=EPOCHS,
        validate_every=VALIDATE_EVERY,
        patience=50,
        return_history=True
    )
    
    # Save model and print progress
    torch.save({"model_state": model.state_dict(), "params": best_params}, CHECKPOINT_PATH)
    
    for i, (train_loss, val_loss, val_auc, val_acc) in enumerate(zip(
        history['train_losses'], history['val_losses'], 
        history['val_aucs'], history['val_accs']
    )):
        if not np.isnan(val_loss):
            epoch = i + 1  # i is already the epoch number
            print(f"Epoch {epoch:4d} | train_loss={train_loss:.5f} | val_loss={val_loss:.5f} | val_auc={val_auc:.4f} | val_acc={val_acc:.4f}")
    
    # Generate predictions and plots
    model.eval()
    with torch.no_grad():
        feats_tensor = torch.from_numpy(X).float().to(device)
        preds = model(feats_tensor).squeeze(1).cpu().numpy()
        probs = (preds + 1.0) / 2.0
    
    # Plot results
    plot_training_curves(
        train_losses=history['train_losses'],
        val_losses=history['val_losses'],
        val_aucs=history['val_aucs'],
        val_accs=history['val_accs'],
        save_path=f"{OUTPUT_DIR}/training_curves.png"
    )
    
    y_true_binary = ((y + 1.0) / 2.0).astype(int)
    plot_roc_curve(y_true_binary, probs, save_path=f"{OUTPUT_DIR}/roc_curve.png")
    plot_predictions_distribution(y_true_binary, probs, save_path=f"{OUTPUT_DIR}/predictions_distribution.png")
    
    # Save predictions
    pred_series = pd.Series(probs, name="prob_signal")
    pred_series.to_pickle(OUTPUT_PICKLE)
    print(f"Saved predictions to {OUTPUT_PICKLE}")

def main():
    """Main function to run hyperparameter optimization and train best model."""
    import argparse
    
    parser = argparse.ArgumentParser(description="MLP Hyperparameter Optimization with Optuna")
    parser.add_argument("--trials", type=int, default=50, help="Number of optimization trials")
    parser.add_argument("--dashboard", action="store_true", help="Start Optuna dashboard (run after optimization)")
    
    args = parser.parse_args()
    
    if args.dashboard:
        print("Starting Optuna dashboard...")
        try:
            optuna_dashboard.run_server(
                storage=OPTUNA_DB_URL,
                host="localhost",
                port=8080
            )
        except KeyboardInterrupt:
            print("Dashboard stopped.")
        except Exception as e:
            print(f"Error starting dashboard: {e}")
        return
    
    print("Starting hyperparameter optimization with Optuna...")
    print(f"Study: {STUDY_NAME}")
    
    # Run optimization
    study, X, y, X_test, y_test, scaler = optimize_hyperparameters(n_trials=args.trials)
    
    # Print optimization results
    print(f"\nOptimization completed!")
    print(f"Number of trials: {len(study.trials)}")
    print(f"Complete trials: {len([t for t in study.trials if t.state == optuna.trial.TrialState.COMPLETE])}")
    
    # Train best model
    print("\nTraining best model...")
    train_best_model(study, X, y, X_test, y_test, scaler)
    
    print("Training completed!")
    print(f"\nTo view the interactive dashboard, run with --dashboard flag")

if __name__ == "__main__":
    main()