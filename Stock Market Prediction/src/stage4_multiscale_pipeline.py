"""
Stage 4: Multi-Scale Continuous Microstructure & Volatility Pipeline
====================================================================

Integrates:
1. Continuous time-series reconstruction (forward-fill price ticks to preserve data integrity).
2. Advanced high-frequency volatility estimators:
   - Instantaneous return variance proxy: r_t^2 (ARCH formulation)
   - Absolute return magnitude: |r_t|
   - Realized Volatility across multi-scale horizons (5, 10, 15, 30, 60 ticks) with min_periods
   - Exponentially Weighted Moving Average (EWMA) volatility (RiskMetrics formulation)
   - Order book normalized range proxy: (max(ask5) - min(bid5)) / mid_price
   - Order book imbalance (OBI) across levels 1-3
   - Cross-asset price ratios and dispersion signals (ETH, BTC, SOL)
3. Evaluates both LightGBM and XGBoost (CUDA Hist) on Pearson correlation.
"""

import pandas as pd
import numpy as np
from sklearn.model_selection import train_test_split
from scipy.stats import pearsonr
import lightgbm as lgb
import xgboost as xgb
import os
import joblib
import warnings
warnings.filterwarnings('ignore')

def load_and_merge(train_dir):
    print("Loading ETH, BTC, and SOL data...")
    eth_path = os.path.join(train_dir, 'ETH.csv')
    if os.path.exists(eth_path):
        df_eth = pd.read_csv(eth_path)
    else:
        df_eth1 = pd.read_csv(os.path.join(train_dir, 'ETH-01.csv'))
        df_eth2 = pd.read_csv(os.path.join(train_dir, 'ETH-02.csv'))
        df_eth = pd.concat([df_eth1, df_eth2], ignore_index=True)
    df_btc = pd.read_csv(os.path.join(train_dir, 'BTC.csv'))
    df_sol = pd.read_csv(os.path.join(train_dir, 'SOL.csv'))

    btc_cols = {c: f"{c}_BTC" for c in df_btc.columns if c != 'timestamp'}
    sol_cols = {c: f"{c}_SOL" for c in df_sol.columns if c != 'timestamp'}

    df_btc = df_btc.rename(columns=btc_cols).drop(columns=['timestamp'])
    df_sol = df_sol.rename(columns=sol_cols).drop(columns=['timestamp'])

    return pd.concat([df_eth, df_btc, df_sol], axis=1)

def engineer_multiscale_features(df):
    print("Engineering multi-scale volatility & microstructure signals...")
    

    for col in df.columns:
        if 'price' in col and df[col].dtype in [np.float64, np.float32]:
            df.loc[(df[col] <= 0.0) | (df[col] > 1e6), col] = np.nan
            df[col] = df[col].ffill()

    for prefix in ['', '_BTC', '_SOL']:

        mid = df[f'mid_price{prefix}'].ffill()
        df[f'mid_price{prefix}'] = mid
        

        ret = np.log(mid / mid.shift(1)).fillna(0)
        df[f'log_return{prefix}'] = ret
        df[f'ret_squared{prefix}'] = ret ** 2
        df[f'abs_return{prefix}'] = ret.abs()
        

        for w in [5, 10, 15, 30, 60]:
            df[f'realized_vol_{w}{prefix}'] = ret.rolling(w, min_periods=2).std().fillna(0)
            

        for span in [10, 30, 60]:
            df[f'ewma_vol_{span}{prefix}'] = np.sqrt(df[f'ret_squared{prefix}'].ewm(span=span).mean()).fillna(0)
            

        for i in range(1, 6):
            df[f'spread{i}{prefix}'] = df[f'ask_price{i}{prefix}'] - df[f'bid_price{i}{prefix}']
            

        for i in range(1, 4):
            bv = df[f'bid_volume{i}{prefix}']
            av = df[f'ask_volume{i}{prefix}']
            df[f'obi{i}{prefix}'] = (bv - av) / (bv + av + 1e-8)
            

        for w in [15, 30]:
            hi = df[f'ask_price5{prefix}'].rolling(w, min_periods=2).max()
            lo = df[f'bid_price5{prefix}'].rolling(w, min_periods=2).min()
            df[f'norm_range_{w}{prefix}'] = ((hi - lo) / (mid + 1e-8)).fillna(0)
            

    for lag in [1, 2, 3]:
        df[f'ret_lag{lag}'] = df['log_return'].shift(lag).fillna(0)
        df[f'rvol10_lag{lag}'] = df['realized_vol_10'].shift(lag).fillna(0)

    df['mid_ratio_ETH_BTC'] = df['mid_price'] / (df['mid_price_BTC'] + 1e-8)
    df['mid_ratio_ETH_SOL'] = df['mid_price'] / (df['mid_price_SOL'] + 1e-8)
    df['ret_diff_ETH_BTC'] = df['log_return'] - df['log_return_BTC']

    return df

engineer_champion_features = engineer_multiscale_features

def clean_and_prepare(df):
    features = [c for c in df.columns if c not in ['timestamp', 'label']]
    df = df.replace([np.inf, -np.inf], np.nan)
    for c in features:
        if df[c].dtype == np.float64:
            df.loc[df[c].abs() > 1e10, c] = np.nan

    X = df[features].astype(np.float32)
    y = df['label'].astype(np.float32)
    

    mask = y.notna() & (X['mid_price'] > 0)
    X = X[mask].replace([np.inf, -np.inf], 0).fillna(0)
    y = y[mask]
    
    return X, y

def main():
    script_dir = os.path.dirname(os.path.abspath(__file__))
    train_dir = os.path.join(script_dir, '..', 'data', 'raw', 'train')
    if not os.path.exists(train_dir):
        train_dir = os.path.join(script_dir, '..', 'data', 'raw', 'train')
    if not os.path.exists(train_dir):
        train_dir = os.path.join(script_dir, '..', 'data', 'raw', 'train')
        
    df = load_and_merge(train_dir)
    df = engineer_multiscale_features(df)
    X, y = clean_and_prepare(df)
    

    X_train, X_val, y_train, y_val = train_test_split(X, y, test_size=0.2, shuffle=False)
    print(f"\nDataset shape: {len(X_train)} Train rows | {len(X_val)} Validation rows | {X.shape[1]} Features")
    

    print("\n--- Training LightGBM Multi-Scale Model ---")
    lgb_model = lgb.LGBMRegressor(
        n_estimators=3000,
        learning_rate=0.04,
        max_depth=6,
        num_leaves=45,
        subsample=0.8,
        colsample_bytree=0.8,
        random_state=42,
        n_jobs=-1
    )
    lgb_model.fit(
        X_train, y_train,
        eval_set=[(X_val, y_val)],
        callbacks=[lgb.early_stopping(50, verbose=False)]
    )
    lgb_preds = lgb_model.predict(X_val)
    lgb_corr, _ = pearsonr(y_val, lgb_preds)
    print(f"LightGBM Pearson Correlation (r): {lgb_corr:.6f} (stopped @ iter {lgb_model.best_iteration_})")

    print("\n--- Training XGBoost Multi-Scale Model (CUDA) ---")
    xgb_model = xgb.XGBRegressor(
        n_estimators=3000,
        learning_rate=0.04,
        max_depth=6,
        subsample=0.8,
        colsample_bytree=0.8,
        tree_method='hist',
        device='cuda',
        early_stopping_rounds=50,
        random_state=42
    )
    xgb_model.fit(
        X_train, y_train,
        eval_set=[(X_val, y_val)],
        verbose=False
    )
    xgb_model.set_params(device='cpu')
    xgb_preds = xgb_model.predict(X_val)
    xgb_corr, _ = pearsonr(y_val, xgb_preds)
    print(f"XGBoost Pearson Correlation (r):  {xgb_corr:.6f} (stopped @ iter {xgb_model.best_iteration})")

    models_dir = os.path.join(script_dir, '..', 'models')
    os.makedirs(models_dir, exist_ok=True)
    
    primary_model = lgb_model if lgb_corr >= xgb_corr else xgb_model
    primary_name = "LightGBM" if lgb_corr >= xgb_corr else "XGBoost"
    best_corr = max(lgb_corr, xgb_corr)
    

    joblib.dump(primary_model, os.path.join(models_dir, 'stage4_multiscale_best.joblib'))
    joblib.dump(list(X.columns), os.path.join(models_dir, 'stage4_multiscale_features.joblib'))
    
    print("\n" + "="*65)
    print("                 BENCHMARK PROGRESSION (r)")
    print("="*65)
    print(f"  Baseline (Raw LOB Features)       :  0.0947")
    print(f"  v2 (Returns + Simple RVol)        :  0.6441")
    print(f"  v3 (Lags + Squared Returns)       :  0.6586")
    print(f"  v4 LightGBM (Continuous + EWMA)  :  {lgb_corr:.6f}")
    print(f"  v4 XGBoost  (Continuous + EWMA)  :  {xgb_corr:.6f}")
    print("="*65)
    print(f"\nSaved Best Model ({primary_name}, r={best_corr:.6f}) to models/stage4_multiscale_best.joblib")

if __name__ == "__main__":
    main()
