import pandas as pd
import numpy as np
from sklearn.model_selection import train_test_split
from sklearn.decomposition import PCA
from sklearn.preprocessing import StandardScaler
from scipy.stats import pearsonr
import lightgbm as lgb
import os
import warnings
warnings.filterwarnings('ignore')

from stage2_realized_volatility import load_and_merge_data, engineer_financial_features, clean_data

def evaluate_model(X_train, y_train, X_val, y_val, name="Model"):

    model = lgb.LGBMRegressor(
        n_estimators=1000, learning_rate=0.05, max_depth=6, 
        num_leaves=31, random_state=42, n_jobs=-1
    )

    model.fit(
        X_train, y_train, 
        eval_set=[(X_val, y_val)], 
        callbacks=[lgb.early_stopping(50, verbose=False)]
    )
    preds = model.predict(X_val)
    corr, _ = pearsonr(y_val, preds)
    print(f"{name:<38} | Pearson r: {corr:.6f} | Features: {X_train.shape[1]}")
    return model, corr

def main():
    script_dir = os.path.dirname(os.path.abspath(__file__))
    train_dir = os.path.join(script_dir, '..', 'datasets', 'train')
    if not os.path.exists(train_dir):
        train_dir = os.path.join(script_dir, '..', 'data', 'raw', 'train')
        
    print("1. Loading and engineering all features...")
    df = load_and_merge_data(
        os.path.join(train_dir, 'ETH.csv'),
        os.path.join(train_dir, 'BTC.csv'),
        os.path.join(train_dir, 'SOL.csv')
    )
    df = engineer_financial_features(df)
    X, y = clean_data(df)
    

    X_train, X_val, y_train, y_val = train_test_split(X, y, test_size=0.2, shuffle=False)
    
    print("\n" + "="*70)
    print("                DIMENSIONALITY REDUCTION BENCHMARK")
    print("="*70)
    

    base_model, base_corr = evaluate_model(X_train, y_train, X_val, y_val, "1. All Features (Baseline)")
    

    k_features = 25
    importances = base_model.feature_importances_
    top_k_idx = np.argsort(importances)[-k_features:]
    top_k_cols = X_train.columns[top_k_idx]
    
    X_train_topk = X_train[top_k_cols]
    X_val_topk = X_val[top_k_cols]
    
    _, topk_corr = evaluate_model(X_train_topk, y_train, X_val_topk, y_val, f"2. Top {k_features} Features (Tree Importance)")
    

    scaler = StandardScaler()
    X_train_scaled = scaler.fit_transform(X_train)
    X_val_scaled = scaler.transform(X_val)
    
    pca_95 = PCA(n_components=0.95, random_state=42)
    X_train_pca = pca_95.fit_transform(X_train_scaled)
    X_val_pca = pca_95.transform(X_val_scaled)
    
    _, pca_corr = evaluate_model(X_train_pca, y_train, X_val_pca, y_val, f"3. PCA (95% Variance Explained)")
    

    pca_fixed = PCA(n_components=k_features, random_state=42)
    X_train_pca_fixed = pca_fixed.fit_transform(X_train_scaled)
    X_val_pca_fixed = pca_fixed.transform(X_val_scaled)
    
    _, pca_fixed_corr = evaluate_model(X_train_pca_fixed, y_train, X_val_pca_fixed, y_val, f"4. PCA (Fixed {k_features} Components)")
    
    print("="*70)
    print("\nConclusion:")
    print("Compare which reduction method retains the highest correlation while dropping the most columns.")

if __name__ == "__main__":
    main()
