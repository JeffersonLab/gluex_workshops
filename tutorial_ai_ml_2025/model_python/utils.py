import os
import random

import matplotlib.pyplot as plt
import numpy as np
from sklearn.metrics import roc_curve, auc
import torch

def set_num_threads(num_cpu):
    os.environ["OMP_NUM_THREADS"] = str(num_cpu)
    os.environ["MKL_NUM_THREADS"] = str(num_cpu)
    torch.set_num_threads(num_cpu)
    try:
        torch.set_num_interop_threads(num_cpu)
    except Exception:
        pass

def set_seed(seed: int) -> None:
    random.seed(seed)
    np.random.seed(seed)
    torch.manual_seed(seed)
    torch.cuda.manual_seed_all(seed)
    torch.backends.cudnn.deterministic = True
    torch.backends.cudnn.benchmark = False

def plot_training_curves(
    train_losses,
    val_losses,
    train_aucs=None,
    val_aucs=None,
    train_accs=None,
    val_accs=None,
    save_path=None,
    figsize=(12, 8),
):
    """
    Plot training curves for loss, AUC, and accuracy.
    """
    n_metrics = 1  # at least loss
    if train_aucs is not None:
        n_metrics += 1
    if train_accs is not None:
        n_metrics += 1
    
    fig, axes = plt.subplots(1, n_metrics, figsize=figsize)
    if n_metrics == 1:
        axes = [axes]
    
    epochs = range(1, len(train_losses) + 1)
    
    # Plot loss
    ax_idx = 0
    axes[ax_idx].plot(epochs, train_losses, 'b-', label='Training Loss', linewidth=2)
    axes[ax_idx].plot(epochs, val_losses, 'r-', label='Validation Loss', linewidth=2)
    axes[ax_idx].set_xlabel('Epoch')
    axes[ax_idx].set_ylabel('Loss')
    axes[ax_idx].set_title('Training and Validation Loss')
    axes[ax_idx].legend()
    axes[ax_idx].grid(True, alpha=0.3)
    
    # Plot AUC if provided
    if train_aucs is not None:
        ax_idx += 1
        axes[ax_idx].plot(epochs, train_aucs, 'b-', label='Training AUC', linewidth=2)
        axes[ax_idx].plot(epochs, val_aucs, 'r-', label='Validation AUC', linewidth=2)
        axes[ax_idx].set_xlabel('Epoch')
        axes[ax_idx].set_ylabel('AUC')
        axes[ax_idx].set_title('Training and Validation AUC')
        axes[ax_idx].legend()
        axes[ax_idx].grid(True, alpha=0.3)
        axes[ax_idx].set_ylim(0, 1)
    
    # Plot Accuracy if provided
    if train_accs is not None:
        ax_idx += 1
        axes[ax_idx].plot(epochs, train_accs, 'b-', label='Training Accuracy', linewidth=2)
        axes[ax_idx].plot(epochs, val_accs, 'r-', label='Validation Accuracy', linewidth=2)
        axes[ax_idx].set_xlabel('Epoch')
        axes[ax_idx].set_ylabel('Accuracy')
        axes[ax_idx].set_title('Training and Validation Accuracy')
        axes[ax_idx].legend()
        axes[ax_idx].grid(True, alpha=0.3)
        axes[ax_idx].set_ylim(0, 1)
    
    plt.tight_layout()
    
    if save_path:
        os.makedirs(os.path.dirname(save_path), exist_ok=True)
        plt.savefig(save_path, dpi=300, bbox_inches='tight')
        print(f"Training curves saved to {save_path}")
    
    plt.show()


def plot_roc_curve(
    y_true,
    y_scores,
    save_path=None,
    figsize=(8, 6),
):
    """
    Plot ROC curve with AUC score.
    """
    fpr, tpr, _ = roc_curve(y_true, y_scores)
    roc_auc = auc(fpr, tpr)
    
    plt.figure(figsize=figsize)
    plt.plot(fpr, tpr, color='darkorange', lw=2, label=f'ROC curve (AUC = {roc_auc:.3f})')
    plt.plot([0, 1], [0, 1], color='navy', lw=2, linestyle='--', label='Random classifier')
    plt.xlim([0.0, 1.0])
    plt.ylim([0.0, 1.05])
    plt.xlabel('False Positive Rate')
    plt.ylabel('True Positive Rate')
    plt.title('Receiver Operating Characteristic (ROC) Curve')
    plt.legend(loc="lower right")
    plt.grid(True, alpha=0.3)
    
    if save_path:
        os.makedirs(os.path.dirname(save_path), exist_ok=True)
        plt.savefig(save_path, dpi=300, bbox_inches='tight')
        print(f"ROC curve saved to {save_path}")
    
    plt.show()


def plot_predictions_distribution(
    y_true,
    y_pred,
    bins=50,
    save_path=None,
    figsize=(12, 5),
):
    """
    Plot distribution of predictions for each class.
    """
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=figsize)
    
    # Plot for class 0 (background)
    class_0_preds = y_pred[y_true == 0]
    ax1.hist(class_0_preds, bins=bins, alpha=0.7, color='red', label='Background (π⁺π⁻)')
    ax1.set_xlabel('Predicted Probability')
    ax1.set_ylabel('Count')
    ax1.set_title('Prediction Distribution - Background')
    ax1.legend()
    ax1.grid(True, alpha=0.3)
    
    # Plot for class 1 (signal)
    class_1_preds = y_pred[y_true == 1]
    ax2.hist(class_1_preds, bins=bins, alpha=0.7, color='blue', label='Signal (K⁺K⁻)')
    ax2.set_xlabel('Predicted Probability')
    ax2.set_ylabel('Count')
    ax2.set_title('Prediction Distribution - Signal')
    ax2.legend()
    ax2.grid(True, alpha=0.3)
    
    plt.tight_layout()
    
    if save_path:
        os.makedirs(os.path.dirname(save_path), exist_ok=True)
        plt.savefig(save_path, dpi=300, bbox_inches='tight')
        print(f"Predictions distribution plot saved to {save_path}")
    
    plt.show()
