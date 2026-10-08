import pandas as pd
import numpy as np
from sklearn.model_selection import train_test_split
from sklearn.metrics import root_mean_squared_error
import xgboost as xgb
import time
import os
import joblib

def load_and_merge_data(eth_path, btc_path, sol_path):
    print("Loading ETH, BTC, and SOL data...")

    # Load datasets
    df_eth = pd.read_csv(eth_path)
    df_btc = pd.read_csv(btc_path)
    df_sol = pd.read_csv(sol_path)

    # Make sure we don't have overlapping column names besides the ones we specifically want
    # Rename BTC and SOL columns (skip timestamp since ETH has the most precise one)
    btc_features = {c: f"{c}_BTC" for c in df_btc.columns if c != 'timestamp'}
    sol_features = {c: f"{c}_SOL" for c in df_sol.columns if c != 'timestamp'}

    df_btc = df_btc.rename(columns=btc_features).drop(columns=['timestamp'])
    df_sol = df_sol.rename(columns=sol_features).drop(columns=['timestamp'])

    # Concatenate horizontally (aligning by row index since they are snapshots at the same time)
    df_merged = pd.concat([df_eth, df_btc, df_sol], axis=1)

    return df_merged

def create_features(df):
    print("Engineering new features...")

    # 1. Spreads (difference between lowest ask and highest bid)
    df['spread_ETH'] = df['ask_price1'] - df['bid_price1']
    df['spread_BTC'] = df['ask_price1_BTC'] - df['bid_price1_BTC']
    df['spread_SOL'] = df['ask_price1_SOL'] - df['bid_price1_SOL']

    # 2. Order Book Imbalances (Level 1)
    # (Bid Vol - Ask Vol) / (Bid Vol + Ask Vol)
    df['obi_ETH'] = (df['bid_volume1'] - df['ask_volume1']) / (df['bid_volume1'] + df['ask_volume1'] + 1e-8)
    df['obi_BTC'] = (df['bid_volume1_BTC'] - df['ask_volume1_BTC']) / (df['bid_volume1_BTC'] + df['ask_volume1_BTC'] + 1e-8)
    df['obi_SOL'] = (df['bid_volume1_SOL'] - df['ask_volume1_SOL']) / (df['bid_volume1_SOL'] + df['ask_volume1_SOL'] + 1e-8)

    # 3. Cross-Asset Price Ratios (gives relative pricing/correlation signals)
    df['price_ratio_ETH_BTC'] = df['mid_price'] / (df['mid_price_BTC'] + 1e-8)
    df['price_ratio_ETH_SOL'] = df['mid_price'] / (df['mid_price_SOL'] + 1e-8)

    return df

def clean_data(df):
    print("Cleaning data (removing NaNs and Infinity overflows)...")
    features = [c for c in df.columns if c not in ['timestamp', 'label']]

    # Cast to float32 first
    X = df[features].astype(np.float32)
    y = df['label'].astype(np.float32)

    # Replace inf with nan
    X = X.replace([np.inf, -np.inf], np.nan)
    y = y.replace([np.inf, -np.inf], np.nan)

    # Drop any row that now contains a NaN
    mask = X.notna().all(axis=1) & y.notna()

    X = X[mask]
    y = y[mask]

    print(f"Kept {len(X)} valid rows after merging and cleaning.")
    return X, y

def main():
    script_dir = os.path.dirname(os.path.abspath(__file__))
    train_dir = os.path.join(script_dir, '..', 'Stock Market Prediction Analysis', 'train')

    eth_path = os.path.join(train_dir, 'ETH.csv')
    btc_path = os.path.join(train_dir, 'BTC.csv')
    sol_path = os.path.join(train_dir, 'SOL.csv')

    # Pipeline
    df = load_and_merge_data(eth_path, btc_path, sol_path)
    df = create_features(df)
    X, y = clean_data(df)

    # Split
    X_train, X_val, y_train, y_val = train_test_split(X, y, test_size=0.2, shuffle=False)

    print(f"\nTraining set: {X_train.shape[0]} rows | Validation set: {X_val.shape[0]} rows")
    print(f"Total features used: {X_train.shape[1]}")

    print("\n--- Training XGBoost with Cross-Asset Features ---")
    start_time = time.time()

    xgb_model = xgb.XGBRegressor(
        n_estimators=10000,
        learning_rate=0.05,
        max_depth=6,
        random_state=42,
        tree_method="hist",
        device="cuda",
        early_stopping_rounds=50
    )

    xgb_model.fit(
        X_train, y_train,
        eval_set=[(X_val, y_val)],
        verbose=100
    )

    xgb_time = time.time() - start_time
    print(f"-> XGBoost stopped at tree {xgb_model.best_iteration}")

    preds = xgb_model.predict(X_val)
    xgb_rmse = root_mean_squared_error(y_val, preds)

    print(f"New XGBoost Total Time: {xgb_time:.2f}s")
    print(f"New XGBoost Best RMSE: {xgb_rmse:.6f}")

    # Save model
    models_dir = os.path.join(script_dir, '..', 'models')
    os.makedirs(models_dir, exist_ok=True)
    joblib.dump(xgb_model, os.path.join(models_dir, 'xgboost_feature_engineered.joblib'))

if __name__ == "__main__":
    main()
