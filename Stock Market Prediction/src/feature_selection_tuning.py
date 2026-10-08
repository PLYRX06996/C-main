import pandas as pd
import numpy as np
from sklearn.model_selection import train_test_split
from sklearn.metrics import root_mean_squared_error
import xgboost as xgb
import optuna
import os
import joblib
import warnings
warnings.filterwarnings('ignore')

from feature_engineering import load_and_merge_data, create_features

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
    train_dir = os.path.join(script_dir, '..', 'data', 'raw', 'train')

    df_full = load_and_merge_data(
        os.path.join(train_dir, 'ETH.csv'),
        os.path.join(train_dir, 'BTC.csv'),
        os.path.join(train_dir, 'SOL.csv')
    )
    df_full = create_features(df_full)

    def cumulative_depth(df, prefix):
        df[f'cum_bid_vol_{prefix}'] = df[[f'bid_volume{i}{prefix}' for i in range(1,6)]].sum(axis=1)
        df[f'cum_ask_vol_{prefix}'] = df[[f'ask_volume{i}{prefix}' for i in range(1,6)]].sum(axis=1)
        df[f'order_book_skew_{prefix}'] = df[f'cum_bid_vol_{prefix}'] / (df[f'cum_ask_vol_{prefix}'] + 1e-8)
        return df

    df_full = cumulative_depth(df_full, '')
    df_full = cumulative_depth(df_full, '_BTC')
    df_full = cumulative_depth(df_full, '_SOL')

    X, y = clean_data(df_full)

    X_train, X_val, y_train, y_val = train_test_split(X, y, test_size=0.2, shuffle=False)

    print("Step 1: Rapid feature importance extraction...")

    fast_model = xgb.XGBRegressor(n_estimators=100, max_depth=5, tree_method='hist', device='cuda', random_state=42)
    fast_model.fit(X_train, y_train)

    feature_importances = pd.DataFrame({
        'feature': X.columns,
        'importance': fast_model.feature_importances_
    }).sort_values('importance', ascending=False)

    top_n = 25
    top_features = feature_importances['feature'].head(top_n).tolist()
    print(f"Top {top_n} features selected. Dropping the other {len(X.columns) - top_n} noisy features.")

    X_train_strict = X_train[top_features]
    X_val_strict = X_val[top_features]
    X_tune = X_train_strict.iloc[-30000:]
    y_tune = y_train.iloc[-30000:]

    print("\nStep 2: Optuna Tuning on Strict Feature Set...")
    def objective(trial):
        params = {
            'n_estimators': 200,
            'learning_rate': trial.suggest_float('learning_rate', 0.005, 0.1, log=True),
            'max_depth': trial.suggest_int('max_depth', 3, 7),
            'min_child_weight': trial.suggest_int('min_child_weight', 1, 10),
            'subsample': trial.suggest_float('subsample', 0.5, 1.0),
            'colsample_bytree': trial.suggest_float('colsample_bytree', 0.5, 1.0),
            'tree_method': 'hist',
            'device': 'cuda',
            'early_stopping_rounds': 20
        }
        model = xgb.XGBRegressor(**params, random_state=42)
        model.fit(X_tune, y_tune, eval_set=[(X_val_strict, y_val)], verbose=False)
        preds = model.predict(X_val_strict)
        return root_mean_squared_error(y_val, preds)

    study = optuna.create_study(direction='minimize')
    study.optimize(objective, n_trials=40)

    print("\nStep 3: Final Ultimate Training on Best Params...")
    best_params = study.best_params
    best_params.update({
        'n_estimators': 15000,
        'tree_method': 'hist',
        'device': 'cuda',
        'early_stopping_rounds': 50
    })

    final_model = xgb.XGBRegressor(**best_params, random_state=42)
    final_model.fit(X_train_strict, y_train, eval_set=[(X_val_strict, y_val)], verbose=100)

    final_preds = final_model.predict(X_val_strict)
    final_rmse = root_mean_squared_error(y_val, final_preds)

    print(f"\n======================================")
    print(f"ABSOLUTE FINAL SCORE LIMIT: {final_rmse:.8f}")

    models_dir = os.path.join(script_dir, '..', 'models')
    os.makedirs(models_dir, exist_ok=True)
    joblib.dump(final_model, os.path.join(models_dir, 'god_mode_xgboost.joblib'))
    joblib.dump(top_features, os.path.join(models_dir, 'top_features_list.joblib'))

if __name__ == "__main__":
    main()
