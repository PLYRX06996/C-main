# Cryptocurrency Limit Order Book Volatility Prediction

An empirical research pipeline predicting 10-second forward rolling volatility of Ethereum (ETH) using high-frequency Level-2 Limit Order Book (LOB) microstructure and cross-asset signals (BTC, SOL).

## Project Overview

In high-frequency cryptocurrency markets, short-horizon volatility forecasting is critical for market making, risk management, and algorithmic execution. This repository implements an end-to-end quantitative machine learning framework that processes 5-level bid/ask depth snapshots sampled at high resolution to model forward realized volatility.

Key components of the methodology:
- **Order Book Imbalance (OBI):** Multi-level volume imbalances capturing directional order flow pressure.
- **Cross-Asset Dynamics:** Joint modeling of Bitcoin (BTC) and Solana (SOL) order books at matching timestamps.
- **Microstructural Noise Filtering:** Early stopping dynamics on tree ensembles to prevent overfitting on tiny volatility targets (~$10^{-5}$).
- **Continuous Multi-Scale Volatility (EWMA):** Exponentially weighted moving average variance memory and normalized order book quote range proxies.
- **Econometric Time-Series Benchmark:** Corsi (2009) Heterogeneous Autoregressive Realized Volatility (HAR-RV) model with Newey-West HAC inference.
- **Pearson Information Coefficient (IC):** Primary evaluation via Pearson correlation ($r$) measuring true directional ranking rather than deceptive scale-only metrics.

## Benchmark Results

### 1. Validation r (Selection-Biased, Chronological 80/20 Split)

*Note on Sample Sizes across Stages:*
Stage 1 (raw ETH): 476,650 / 119,163 (35,478 initial NaN rows dropped). Stages 2–3 (3-asset merge, `dropna`): 186,070 / 46,518. Stages 4–7 (continuous pipeline, 135 features): 484,750 / 121,188 of 605,938 usable rows (631,292 raw). Rigorous benchmark: 424,156 / 90,890 / 90,892.

| Stage / Experiment | Model Architecture | Configuration / Feature Strategy | Validation RMSE | Pearson Correlation ($r$) |
| :--- | :--- | :--- | :---: | :---: |
| **Stage 1: Raw Baselines** | Random Forest | Raw 5-level order book depth | 0.000386 | -0.0189 |
| | LightGBM Baseline | Raw 5-level order book depth | 0.000162 | +0.0947 |
| | XGBoost Baseline | Raw 5-level order book depth | 0.000133 | +0.0887 |
| **Stage 2: Feature Engineering** | LightGBM | Multi-horizon realized vol (5–30), spreads, returns, OBI | 0.000078 | +0.6442 |
| **Stage 3: Econometric Signals** | LightGBM (Tuned) | Squared returns, absolute returns, lag volatility | 0.000076 | +0.6558* |
| | XGBoost (CUDA) | Econometric volatility features | 0.000079 | +0.6440* |
| **Stage 4: Continuous Signals** | LightGBM | Continuous tick depth, multi-scale EWMA vol, quote range proxy | 0.000057 | +0.6811 |
| | XGBoost (CUDA) | Continuous tick depth, multi-scale EWMA vol, quote range proxy | 0.000057 | +0.6851 |
| **Stage 5: Linear Ensemble** | SLSQP Blend | 5-Model Suite (19.9% LGBM-C + 20.4% LGBM-Deep + 59.7% XGB-C) | 0.000056 | +0.6910 |
| **Stage 6: Stacking Ensemble** | Corrected Meta-Learner | 3-Fold TimeSeriesSplit out-of-fold meta-features (Ridge: 0.6895, XGB: 0.6615) | 0.000056 | +0.6910 |
| **Stage 7: Optuna Ensemble** | Direct Optuna Ensemble | Pearson r direct optimization (20.7% LGBM Tuned + 58.3% XGB Tuned + 21.1% LGBM Deep) | 0.000056 | +0.6914 |
| **Stage 8: Canonical HAR-RV** | Corsi (2009) OLS | 4-horizon volatility cascade (5, 15, 60 ticks + shock) | 0.000057 | +0.6935 |
| **Stage 8: Regularized HAR-style** | RidgeCV (30 Vol) | Multi-asset multi-horizon realized vol + EWMA (ETH, BTC, SOL) | 0.000056 | +0.7071 |

\*\*Note: Early exploratory Optuna trials showed $\pm 0.003$ run-to-run variation prior to pinning deterministic random seeds (`seed=42`).*

---

### 2. Rigorous 3-Way Chronological Benchmark (70% Train / 15% Validation / 15% Test)
Evaluated on strictly held-out chronological test observations (90,892 samples) to evaluate out-of-sample generalization.

| Model / Strategy | Validation $r$ | Validation RMSE | Out-of-Sample Test $r$ | Out-of-Sample Test RMSE | Out-of-Sample Test QLIKE |
| :--- | :---: | :---: | :---: | :---: | :---: |
| Baseline 1: Persistence (`rvol10`) | 0.6996 | 6.998e-5 | 0.5441 | 6.200e-5 | 1.519e+6 |
| Baseline 2: Univariate OLS (`rvol10`) | 0.6996 | 7.080e-5 | 0.5441 | 6.049e-5 | 0.7297 |
| Canonical HAR-RV (Corsi 2009 OLS) | 0.7740 | 5.844e-5 | 0.6710 | 5.002e-5 | 0.7218 |
| Regularized multi-asset HAR-style Ridge (30 Vol) | 0.7866 | 5.627e-5 | 0.6860 | 4.841e-5 | 0.7111 |
| LightGBM Conservative (Depth 4) | 0.7636 | 5.942e-5 | 0.6695 | 5.343e-5 | 0.7248 |
| XGBoost Conservative (Depth 4) | 0.7360 | 6.557e-5 | 0.6749 | 4.981e-5 | 0.7183 |
| LightGBM Deep (Depth 6) | 0.7543 | 6.065e-5 | 0.6459 | 5.572e-5 | 0.7380 |
| XGBoost Deep (Depth 6) | 0.7373 | 6.559e-5 | 0.6764 | 4.988e-5 | 0.7171 |
| Equal-Weight Tree Blend (50/50 LGBM + XGB) | 0.7583 | 6.161e-5 | 0.6828 | 4.899e-5 | 0.7135 |
| Stage 5: SLSQP Tree Blend (Val Tuned) | 0.7636 | 5.950e-5 | 0.6711 | 5.299e-5 | 0.7239 |
| LightGBM Tuned (Stage 7) | 0.7604 | 6.033e-5 | 0.6737 | 5.172e-5 | 0.7210 |
| XGBoost Tuned (Stage 7) | 0.7376 | 6.555e-5 | 0.6752 | 4.990e-5 | 0.7180 |
| Econometric-Tree Hybrid (1/3 Ridge + 1/3 LGB + 1/3 XGB) | 0.7841 | 5.840e-5 | 0.6887 | 4.797e-5 | 0.7092 |

---

### 3. Pipeline Architecture & Script Mapping

| Pipeline Stage | Script | Key Artifact | Metric Tracked |
| :--- | :--- | :--- | :--- |
| **Stage 1: Baselines** | `src/stage1_train_baselines.py` | `models/*_baseline.joblib` | RMSE / MAE |
| **Stage 1: Early Stopping** | `src/stage1_early_stopping.py` | `models/*_early_stop.joblib` | Early-stopped RMSE |
| **Stage 1: Overfitting Proof**| `src/stage1_overfitting_proof.py`| Console output | Iteration degradation |
| **Stage 2: Cross-Asset** | `src/stage2_cross_asset_features.py`| `models/xgboost_feature_engineered.joblib` | Early-stopped RMSE |
| **Stage 2: Realized Vol** | `src/stage2_realized_volatility.py`| Console output | Pearson $r$ |
| **Stage 3: Econometric Signals**| `src/stage2_volatility_clustering.py`| `models/stage2_best_model.joblib` | Pearson $r$ |
| **Stage 3: Feature Selection** | `src/stage3_feature_selection_tuning.py` | `models/top_features_list.joblib` | Optuna RMSE |
| **Stage 4: Continuous Signals**| `src/stage4_multiscale_pipeline.py`| `models/stage4_multiscale_best.joblib` | Pearson $r$ |
| **Stage 5: Linear Blend** | `src/stage5_model_ensemble.py` | `models/stage5_optimal_weights.joblib` | Pearson $r$ |
| **Stage 6: Stacking Ensemble**| `src/stage6_stacking_ensemble.py`| `models/stage6_meta_ridge.joblib` | Pearson $r$ |
| **Stage 7: Optuna Ensemble** | `src/stage7_optuna_ensemble.py` | `models/stage7_optimal_weights.joblib` | Pearson $r$ |
| **Stage 8: Econometric HAR-RV**| `src/stage8_har_rv_timeseries.py`| `models/stage8_har_rv_best.joblib` | Pearson $r$ & QLIKE |
| **3-Way Benchmark** | `src/eval_rigorous_test_benchmark.py` | `models/rigorous_benchmark_3way_results.csv` | Pearson $r$, RMSE, QLIKE |
| **Data Integrity Verification**| `src/verify_data_integrity.py` | Console verification | Alignment & Stationarity |

---

### 4. Integrity Notes & Empirical Limitations

1. **Chronological Partitions Only:** All splits are strictly chronological (`shuffle=False`). No randomized k-fold cross-validation is used.
2. **Held-Out Test Set:** The out-of-sample "test" slice is the final 15% of the chronological dataset (90,892 observations).
3. **Contemporaneous Return in Label:** The forward 10-second rolling realized volatility label window includes the contemporaneous return $r_t$; features contain no future information.
4. **Tick Spacing:** Median and mean time delta between consecutive observations is verified at exactly 1.0 second.
5. **Cross-Asset Alignment:** BTC and SOL feeds are verified to match ETH at minute-level resolution; row-level ordering within each minute follows snapshot sequencing.
6. **Top Model Differences:** The difference between the Econometric-Tree Hybrid ($r=0.6887$) and Regularized HAR-style Ridge ($r=0.6860$) is $\le 0.003$, which is not tested for significance.
7. **Regime Degradation:** The performance drop from validation to test reflects a macro market volatility regime transition (persistence dropped from $0.6996 \rightarrow 0.5441$).

## Repository Structure

```text
├── data/
│   ├── raw/                 # Raw L2 order book CSVs (train/test) [Git-ignored]
│   └── processed/           # Processed artifacts and cache
├── docs/
│   ├── methodology.md       # Empirical design, Pearson IC & validation strategy
│   ├── features.md          # Mathematical feature formulas, OBI, EWMA volatility
│   ├── data_dictionary.md   # Order book column definitions
│   └── concepts_log.md      # Quantitative finance & ML concept mastery guide
├── logs/                    # Reproducibility run logs
├── models/                  # Serialized models and benchmark results
├── src/                     # Core pipeline source code (Stages 1-8)
├── tests/                   # Verification and test suites
├── app.py                   # Streamlit quantitative microstructure terminal
├── run_all.sh               # Sequential stage runner
├── requirements.txt         # Pinned project dependencies
└── .gitignore               # Strict ignore rules
```

## Setup & Quickstart

### Prerequisites
Python 3.10+ (tested with Python 3.14 on Linux). NVIDIA CUDA acceleration recommended for XGBoost training (`device='cuda'`); the interactive diagnostic dashboard runs on CPU.

```bash
# Clone the repository
git clone https://github.com/plyrX06996/Stock-Market-Prediction-Analysis.git
cd "Stock Market Prediction"

# Create and activate virtual environment
python -m venv .venv
source .venv/bin/activate

# Install dependencies
pip install -r requirements.txt
```

### Reproducing Benchmark Scores
To execute all stages sequentially:
```bash
./run_all.sh all
```

To run the rigorous 3-way benchmark:
```bash
python src/eval_rigorous_test_benchmark.py
```

To launch the interactive quantitative terminal:
```bash
streamlit run app.py
```
