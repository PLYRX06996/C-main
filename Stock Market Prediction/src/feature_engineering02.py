import pandas as pd
import numpy as np
from sklearn.model_selection import train_test_split
from scipy.stats import pearsonr
import lightgbm as lgb
import time
import os
import joblib
import warnings

warnings.filterwarnings('ignore')

def load_and_merge_data(eth_path, btc_path, sol_path):
    print("Loading datasets...")
    df_eth = pd.read_csv(eth_path)
    df_btc = pd.read_csv(btc_path)
    df_sol = pd.read_csv(sol_path)

    btc_features = {c: f"{c}_BTC" for c in df_btc.columns if c != 'timestamp'}
    sol_features = {c: f"{c}_SOL" for c in df_sol.columns if c != 'timestamp'}

    df_btc = df_btc.rename(columns=btc_features).drop(columns=['timestamp'])
    df_sol = df_sol.rename(columns=sol_features).drop(columns=['timestamp'])

    df_merged = pd.concat([df_eth, df_btc, df_sol], axis=1)
    return df_merged

def engineer_financial_features(df):
    print("Engineering advanced financial features (Spreads, Returns, Volatility, OBI)...")
    

    for prefix in ['', '_BTC', '_SOL']:
        df[f'spread1{prefix}'] = df[f'ask_price1{prefix}'] - df[f'bid_price1{prefix}']
        df[f'spread2{prefix}'] = df[f'ask_price2{prefix}'] - df[f'bid_price2{prefix}']
    

    for prefix in ['', '_BTC', '_SOL']:
        mid_col = f'mid_price{prefix}'
        df[f'log_return{prefix}'] = np.log(df[mid_col] / df[mid_col].shift(1))
        

        for window in [5, 15, 30]:
            df[f'realized_vol_{window}{prefix}'] = df[f'log_return{prefix}'].rolling(window=window).std()
            

    for prefix in ['', '_BTC', '_SOL']:
        for i in range(1, 4):
            bid_v = df[f'bid_volume{i}{prefix}']
            ask_v = df[f'ask_volume{i}{prefix}']
            df[f'obi{i}{prefix}'] = (bid_v - ask_v) / (bid_v + ask_v + 1e-8)
            
    return df

def clean_data(df):
    print("Cleaning NaN and Inf values (caused by rolling windows and division)...")
    features = [c for c in df.columns if c not in ['timestamp', 'label']]
    

    X = df[features].astype(np.float32)
    y = df['label'].astype(np.float32)
    
    X = X.replace([np.inf, -np.inf], np.nan)
    y = y.replace([np.inf, -np.inf], np.nan)
    
    mask = X.notna().all(axis=1) & y.notna()
    return X[mask], y[mask]

def main():
    script_dir = os.path.dirname(os.path.abspath(__file__))
    train_dir = os.path.join(script_dir, '..', 'datasets', 'train')
    if not os.path.exists(train_dir):
        train_dir = os.path.join(script_dir, '..', 'data', 'raw', 'train')
        
    eth_path = os.path.join(train_dir, 'ETH.csv')
    btc_path = os.path.join(train_dir, 'BTC.csv')
    sol_path = os.path.join(train_dir, 'SOL.csv')
    
    df = load_and_merge_data(eth_path, btc_path, sol_path)
    df = engineer_financial_features(df)
    X, y = clean_data(df)
    

    X_train, X_val, y_train, y_val = train_test_split(X, y, test_size=0.2, shuffle=False)
    print(f"\nTraining on {len(X_train)} rows, Validation on {len(X_val)} rows.")
    print(f"Total features used: {X_train.shape[1]}")
    
    print("\n--- Evaluating Feature Engineered Model (LightGBM) ---")

    lgb_model = lgb.LGBMRegressor(
        n_estimators=1000, 
        learning_rate=0.05, 
        max_depth=6, 
        num_leaves=31,
        random_state=42, 
        n_jobs=-1
    )
    
    lgb_model.fit(
        X_train, y_train,
        eval_set=[(X_val, y_val)],
        callbacks=[lgb.early_stopping(50, verbose=False)]
    )
    
    preds = lgb_model.predict(X_val)
    correlation, p_val = pearsonr(y_val, preds)
    
    print("\n" + "="*60)
    print(f"Post-Engineering Pearson Correlation (r): {correlation:.6f}")
    print("="*60)
    

    importances = lgb_model.feature_importances_
    imp_df = pd.DataFrame({'feature': X.columns, 'importance': importances})
    imp_df = imp_df.sort_values('importance', ascending=False).head(10)
    print("\nTop 10 Most Important Engineered Features:")
    print(imp_df.to_string(index=False))

if __name__ == "__main__":
    main()
