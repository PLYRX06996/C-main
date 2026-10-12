import pandas as pd
import numpy as np
from sklearn.model_selection import train_test_split
from scipy.stats import pearsonr
import lightgbm as lgb
import os
import warnings
warnings.filterwarnings('ignore')

def load_and_merge(train_dir):
    df_eth = pd.read_csv(os.path.join(train_dir, 'ETH.csv'))
    df_btc = pd.read_csv(os.path.join(train_dir, 'BTC.csv'))
    df_sol = pd.read_csv(os.path.join(train_dir, 'SOL.csv'))

    btc_cols = {c: f"{c}_BTC" for c in df_btc.columns if c != 'timestamp'}
    sol_cols = {c: f"{c}_SOL" for c in df_sol.columns if c != 'timestamp'}

    df_btc = df_btc.rename(columns=btc_cols).drop(columns=['timestamp'])
    df_sol = df_sol.rename(columns=sol_cols).drop(columns=['timestamp'])

    return pd.concat([df_eth, df_btc, df_sol], axis=1)

def build_proper_features(df):
    print("Building continuous financial features...")
    

    for prefix in ['', '_BTC', '_SOL']:
        mid = df[f'mid_price{prefix}'].ffill()
        df[f'mid_price{prefix}'] = mid
        

        ret = np.log(mid / mid.shift(1)).fillna(0)
        df[f'log_return{prefix}'] = ret
        df[f'ret_squared{prefix}'] = ret ** 2
        df[f'abs_return{prefix}'] = ret.abs()
        

        for w in [5, 10, 15, 30, 60]:
            df[f'realized_vol_{w}{prefix}'] = ret.rolling(w, min_periods=2).std().bfill().fillna(0)
            

        for i in range(1, 6):
            df[f'spread{i}{prefix}'] = df[f'ask_price{i}{prefix}'] - df[f'bid_price{i}{prefix}']
            

        for i in range(1, 4):
            bv = df[f'bid_volume{i}{prefix}']
            av = df[f'ask_volume{i}{prefix}']
            df[f'obi{i}{prefix}'] = (bv - av) / (bv + av + 1e-8)
            

    for lag in [1, 2, 3]:
        df[f'ret_lag{lag}'] = df['log_return'].shift(lag).fillna(0)
        df[f'rvol10_lag{lag}'] = df['realized_vol_10'].shift(lag).bfill().fillna(0)

    df['mid_ratio_ETH_BTC'] = df['mid_price'] / (df['mid_price_BTC'] + 1e-8)
    df['ret_diff_ETH_BTC'] = df['log_return'] - df['log_return_BTC']

    return df

def main():
    script_dir = os.path.dirname(os.path.abspath(__file__))
    train_dir = os.path.join(script_dir, '..', 'datasets', 'train')
    
    df = load_and_merge(train_dir)
    df = build_proper_features(df)
    

    features = [c for c in df.columns if c not in ['timestamp', 'label']]
    X = df[features].astype(np.float32)
    y = df['label'].astype(np.float32)
    

    mask = y.notna() & (X['mid_price'] > 0)
    X = X[mask].fillna(0)
    y = y[mask]
    
    print(f"Total usable continuous rows: {len(X)} (Features: {X.shape[1]})")
    
    X_train, X_val, y_train, y_val = train_test_split(X, y, test_size=0.2, shuffle=False)
    print(f"Train: {len(X_train)} rows | Validation: {len(X_val)} rows")
    

    print("\n--- Training LightGBM on Full Continuous Dataset ---")
    model = lgb.LGBMRegressor(
        n_estimators=3000,
        learning_rate=0.04,
        max_depth=6,
        num_leaves=45,
        subsample=0.8,
        colsample_bytree=0.8,
        random_state=42,
        n_jobs=-1
    )
    
    model.fit(
        X_train, y_train,
        eval_set=[(X_val, y_val)],
        callbacks=[lgb.early_stopping(50, verbose=False)]
    )
    
    preds = model.predict(X_val)
    corr, p_val = pearsonr(y_val, preds)
    
    print("\n" + "="*55)
    print(f"CONTINUOUS DATASET PEARSON CORRELATION: {corr:.6f}")
    print(f"Stopped at iteration: {model.best_iteration_}")
    print("="*55)
    

    imp = pd.DataFrame({'feature': X.columns, 'importance': model.feature_importances_})
    print("\nTop 15 Features:")
    print(imp.sort_values('importance', ascending=False).head(15).to_string(index=False))

if __name__ == "__main__":
    main()
