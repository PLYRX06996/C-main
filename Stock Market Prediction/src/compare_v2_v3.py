"""
Fair head-to-head comparison of v2 vs v3 features on the SAME data split.
Both pipelines use identical cleaning: drop first 60 warm-up rows, fillna(0).
"""

import pandas as pd
import numpy as np
from sklearn.model_selection import train_test_split
from scipy.stats import pearsonr
import lightgbm as lgb
import os
import warnings
warnings.filterwarnings('ignore')

from feature_engineering02 import load_and_merge_data, engineer_financial_features
from feature_engineering03 import add_targeted_features

def clean_consistent(df):
    """Consistent cleaning for fair comparison."""
    features = [c for c in df.columns if c not in ['timestamp', 'label']]
    df = df.iloc[60:].copy()
    X = df[features].astype(np.float32)
    y = df['label'].astype(np.float32)
    X = X.replace([np.inf, -np.inf], np.nan).fillna(0)
    y = y.replace([np.inf, -np.inf], np.nan)
    mask = y.notna()
    return X[mask], y[mask]

def train_and_eval(X_train, y_train, X_val, y_val, label):
    model = lgb.LGBMRegressor(
        n_estimators=1000, learning_rate=0.05, max_depth=6,
        num_leaves=31, random_state=42, n_jobs=-1
    )
    model.fit(
        X_train, y_train, eval_set=[(X_val, y_val)],
        callbacks=[lgb.early_stopping(50, verbose=False)]
    )
    preds = model.predict(X_val)
    corr, _ = pearsonr(y_val, preds)
    print(f"  {label:<40} | r = {corr:.6f} | {X_train.shape[1]} feats | stopped @ iter {model.best_iteration_}")
    return corr

def main():
    script_dir = os.path.dirname(os.path.abspath(__file__))
    train_dir = os.path.join(script_dir, '..', 'datasets', 'train')
    if not os.path.exists(train_dir):
        train_dir = os.path.join(script_dir, '..', 'data', 'raw', 'train')

    df_raw = load_and_merge_data(
        os.path.join(train_dir, 'ETH.csv'),
        os.path.join(train_dir, 'BTC.csv'),
        os.path.join(train_dir, 'SOL.csv')
    )

    print("\nBuilding v2 features...")
    df_v2 = engineer_financial_features(df_raw.copy())
    X_v2, y_v2 = clean_consistent(df_v2)
    X_train_v2, X_val_v2, y_train_v2, y_val_v2 = train_test_split(X_v2, y_v2, test_size=0.2, shuffle=False)

    print("Building v3 features...")
    df_v3 = engineer_financial_features(df_raw.copy())
    df_v3 = add_targeted_features(df_v3)
    X_v3, y_v3 = clean_consistent(df_v3)
    X_train_v3, X_val_v3, y_train_v3, y_val_v3 = train_test_split(X_v3, y_v3, test_size=0.2, shuffle=False)

    print(f"\nData sizes — v2: {len(X_v2)} rows, v3: {len(X_v3)} rows")
    print(f"Val sizes  — v2: {len(X_val_v2)} rows, v3: {len(X_val_v3)} rows")

    print("\n" + "="*75)
    print("               HEAD-TO-HEAD COMPARISON (Same Data)")
    print("="*75)
    corr_v2 = train_and_eval(X_train_v2, y_train_v2, X_val_v2, y_val_v2, "v2: Spreads + Returns + RVol + OBI")
    corr_v3 = train_and_eval(X_train_v3, y_train_v3, X_val_v3, y_val_v3, "v3: v2 + Lags + Squared + AbsRet + RVol60")
    print("="*75)
    delta = corr_v3 - corr_v2
    print(f"\n  Δ Improvement: {delta:+.6f} ({'✅ v3 wins' if delta > 0 else '⚠️ v2 still better'})")

if __name__ == "__main__":
    main()
