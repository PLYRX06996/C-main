# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview & Current State (Updated Oct 2026)

Stock/crypto market prediction analysis project. The goal is to predict **10-second ahead rolling volatility** of Ethereum (ETH) using Level 2 order book data (5-level bid/ask depth) across three cryptocurrency pairs: **BTC**, **ETH**, and **SOL**.

**Current Progress:** We have successfully built an advanced, automated Machine Learning pipeline. The baseline models have been established, feature engineered, and heavily tuned via Bayesian Optimization (Optuna).

### Final Benchmark Scores (RMSE on 20% Time-Series splits)
* XGBoost (GPU): `0.00007876` - (Absolute best model)
* LightGBM (CPU): `0.00008427`
* Random Forest (CPU): `0.00061858`

## Directory Structure

```
Stock Market Prediction Analysis/
├── train/          # ~631K rows (Used for training/validation splits)
├── test/           # ~270K rows (Final exam sets - DO NOT HAVE LABELS)
src/
├── baseline_models.py           # Basic model tests on raw 21 features
├── early_stop_models.py         # Introduces early stopping on XGB/LGB to prevent overfitting
├── force_1000.py                # Script to prove overfitting (forces 1000 iterations)
├── feature_engineering.py       # Combines BTC+ETH+SOL, adds spreads/ratios/OBIs
├── feature_selection_tuning.py  # Optuna Bayesian Search + Strict top-25 feature selection
└── final_evaluation_all.py      # Generates final leaderboard scores
models/
├── god_mode_xgboost.joblib      # The ultimate tuned final XGBoost model
└── top_features_list.joblib     # The exact 25 features required by the ultimate model
```

## Data Schema & Critical Insights

- **The Target `label`:** The `label` column in `train/ETH.csv` has been confirmed via math to be the **10-second forward rolling standard deviation of logarithmic returns** (10-second volatility). The label is extremely tiny (e.g., `6.0e-05`), so models must use Early Stopping quickly (usually between iteration 5 and 50) to avoid instantly overfitting to noise.
- **Timestamp Strategy:** The datasets (ETH, BTC, SOL) have perfectly aligned row counts (631,293) representing the same exact snapshot in time. They must be merged horizontally (row-by-row) via `pd.concat(axis=1)`. Do not attempt to merge on timestamp strings. 
- **Missing values**: SOL training data contains exact rows of NaNs. Features must be generated first, then rows containing any `np.inf` or `NaN` across the master dataset should be dropped to maintain chronological alignment.
- **Top Predictive Features:** The Optuna framework discovered that Bitcoin's Ask Prices (`ask_price1_BTC`, `ask_price2_BTC`) are phenomenally strong leading indicators for Ethereum's volatility. Solana data added almost zero predictive value and was mostly ignored by the models.

## Development Environment
- Python version 3.14 via local `.venv/`
- Hardware: NVIDIA GeForce RTX 2050 (4GB) is available and functional for CUDA `xgboost`.
