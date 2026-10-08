import pandas as pd
import numpy as np
from sklearn.model_selection import train_test_split
from sklearn.metrics import root_mean_squared_error
from sklearn.ensemble import RandomForestRegressor
import xgboost as xgb
import lightgbm as lgb
import optuna
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

    return pd.concat([df_eth, df_btc, df_sol], axis=1)

def create_features(df):
    print("Engineering advanced correlation features...")

    # Base spreads and imbalances for all 5 levels for ETH
    for i in range(1, 6):
        df[f'spread_ETH_{i}'] = df[f'ask_price{i}'] - df[f'bid_price{i}']
        df[f'obi_ETH_{i}'] = (df[f'bid_volume{i}'] - df[f'ask_volume{i}']) / (df[f'bid_volume{i}'] + df[f'ask_volume{i}'] + 1e-8)

        df[f'spread_BTC_{i}'] = df[f'ask_price{i}_BTC'] - df[f'bid_price{i}_BTC']
        df[f'obi_BTC_{i}'] = (df[f'bid_volume{i}_BTC'] - df[f'ask_volume{i}_BTC']) / (df[f'bid_volume{i}_BTC'] + df[f'ask_volume{i}_BTC'] + 1e-8)

        df[f'spread_SOL_{i}'] = df[f'ask_price{i}_SOL'] - df[f'bid_price{i}_SOL']
        df[f'obi_SOL_{i}'] = (df[f'bid_volume{i}_SOL'] - df[f'ask_volume{i}_SOL']) / (df[f'bid_volume{i}_SOL'] + df[f'ask_volume{i}_SOL'] + 1e-8)

    # Cross-asset relationships
    df['mid_ratio_ETH_BTC'] = df['mid_price'] / (df['mid_price_BTC'] + 1e-8)
    df['mid_ratio_ETH_SOL'] = df['mid_price'] / (df['mid_price_SOL'] + 1e-8)

    # Weighted Mid Price (WMP)
    df['wmp_ETH'] = (df['bid_price1'] * df['ask_volume1'] + df['ask_price1'] * df['bid_volume1']) / (df['bid_volume1'] + df['ask_volume1'] + 1e-8)
    df['wmp_BTC'] = (df['bid_price1_BTC'] * df['ask_volume1_BTC'] + df['ask_price1_BTC'] * df['bid_volume1_BTC']) / (df['bid_volume1_BTC'] + df['ask_volume1_BTC'] + 1e-8)

    # Momentum approximation (diff from previous row)
    # Be careful not to leak future, use shift(1)
    df['mid_price_diff_ETH'] = df['mid_price'].diff()
    df['mid_price_diff_BTC'] = df['mid_price_BTC'].diff()
    df['mid_price_diff_SOL'] = df['mid_price_SOL'].diff()

    return df

def clean_data(df):
    features = [c for c in df.columns if c not in ['timestamp', 'label']]
    X = df[features].astype(np.float32)
    y = df['label'].astype(np.float32)

    X = X.replace([np.inf, -np.inf], np.nan)
    y = y.replace([np.inf, -np.inf], np.nan)

    mask = X.notna().all(axis=1) & y.notna()
    X = X[mask]
    y = y[mask]

    return X, y

def main():
    script_dir = os.path.dirname(os.path.abspath(__file__))
    train_dir = os.path.join(script_dir, '..', 'Stock Market Prediction Analysis', 'train')

    df_full = load_and_merge_data(
        os.path.join(train_dir, 'ETH.csv'),
        os.path.join(train_dir, 'BTC.csv'),
        os.path.join(train_dir, 'SOL.csv')
    )
    df_full = create_features(df_full)
    X, y = clean_data(df_full)

    X_train, X_val, y_train, y_val = train_test_split(X, y, test_size=0.2, shuffle=False)

    # Because tuning is slow, we will only take the last 20,000 continuous rows for fast Optuna tuning
    # Once best params are found, we train on the full set.
    X_tune_train = X_train.iloc[-20000:]
    y_tune_train = y_train.iloc[-20000:]

    # -------------------------------------------------------------
    # Optuna XGBoost Tuning
    # -------------------------------------------------------------
    print("\n--- Tuning XGBoost ---")
    def xgb_objective(trial):
        params = {
            'n_estimators': 200, # fast proxy
            'learning_rate': trial.suggest_float('learning_rate', 0.01, 0.2, log=True),
            'max_depth': trial.suggest_int('max_depth', 3, 9),
            'colsample_bytree': trial.suggest_float('colsample_bytree', 0.5, 1.0),
            'subsample': trial.suggest_float('subsample', 0.5, 1.0),
            'tree_method': 'hist',
            'device': 'cuda',
            'early_stopping_rounds': 20
        }
        model = xgb.XGBRegressor(**params, random_state=42)
        model.fit(X_tune_train, y_tune_train, eval_set=[(X_val, y_val)], verbose=False)
        preds = model.predict(X_val)
        return root_mean_squared_error(y_val, preds)

    xgb_study = optuna.create_study(direction='minimize')
    xgb_study.optimize(xgb_objective, n_trials=30)  # fast 30 trials
    print(f"Best XGB Tuning RMSE: {xgb_study.best_value}")

    # -------------------------------------------------------------
    # Optuna LightGBM Tuning
    # -------------------------------------------------------------
    print("\n--- Tuning LightGBM ---")
    def lgb_objective(trial):
        params = {
            'n_estimators': 200,
            'learning_rate': trial.suggest_float('learning_rate', 0.01, 0.2, log=True),
            'max_depth': trial.suggest_int('max_depth', 3, 9),
            'num_leaves': trial.suggest_int('num_leaves', 20, 100),
            'colsample_bytree': trial.suggest_float('colsample_bytree', 0.5, 1.0),
            'subsample': trial.suggest_float('subsample', 0.5, 1.0),
            'n_jobs': -1
        }
        model = lgb.LGBMRegressor(**params, random_state=42)
        model.fit(
            X_tune_train, y_tune_train,
            eval_set=[(X_val, y_val)],
            callbacks=[lgb.early_stopping(10, verbose=False)]
        )
        preds = model.predict(X_val)
        return root_mean_squared_error(y_val, preds)

    lgb_study = optuna.create_study(direction='minimize')
    lgb_study.optimize(lgb_objective, n_trials=30)
    print(f"Best LGB Tuning RMSE: {lgb_study.best_value}")

    # -------------------------------------------------------------
    # Randomized Random Forest (Proxy)
    # -------------------------------------------------------------
    print("\n--- Tuning Random Forest (Proxy) ---")
    # RF is too slow, we'll try just 3 setups and take the best
    rf_best_score = float('inf')
    best_rf_params = {}
    for max_depth in [5, 10, None]:
        rf = RandomForestRegressor(n_estimators=50, max_depth=max_depth, n_jobs=-1, random_state=42)
        rf.fit(X_tune_train, y_tune_train)
        preds = rf.predict(X_val)
        score = root_mean_squared_error(y_val, preds)
        if score < rf_best_score:
            rf_best_score = score
            best_rf_params = {'max_depth': max_depth}
    print(f"Best RF Tuning RMSE: {rf_best_score}")

    # =============================================================
    # Train Best XGBoost on FULL Dataset
    # =============================================================
    print("\n[====== RETRAINING BEST MODEL ON FULL DATASET ======]")
    best_xgb_params = xgb_study.best_params
    best_xgb_params.update({
        'n_estimators': 10000,
        'tree_method': 'hist',
        'device': 'cuda',
        'early_stopping_rounds': 50
    })

    final_xgb = xgb.XGBRegressor(**best_xgb_params, random_state=42)
    final_xgb.fit(X_train, y_train, eval_set=[(X_val, y_val)], verbose=100)

    final_preds = final_xgb.predict(X_val)
    final_rmse = root_mean_squared_error(y_val, final_preds)
    print(f"*** FINAL XGBOOST SCORE: {final_rmse:.8f} ***")

    # Feature Importance Analysis
    importances = final_xgb.feature_importances_
    features_list = X.columns
    f_imp = pd.DataFrame({'feature': features_list, 'importance': importances})
    f_imp = f_imp.sort_values('importance', ascending=False).head(15)
    print("\nTop 15 Most Important Features:")
    print(f_imp)

    models_dir = os.path.join(script_dir, '..', 'models')
    joblib.dump(final_xgb, os.path.join(models_dir, 'ultimate_xgboost.joblib'))

if __name__ == "__main__":
    main()
