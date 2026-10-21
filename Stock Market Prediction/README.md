# Cryptocurrency Limit Order Book Volatility Prediction

An empirical research pipeline predicting 10-second forward rolling volatility of Ethereum (ETH) using high-frequency Level-2 Limit Order Book (LOB) microstructure and cross-asset signals (BTC, SOL).

## Project Overview

In high-frequency cryptocurrency markets, short-horizon volatility forecasting is critical for market making, risk management, and algorithmic execution. This repository implements an end-to-end quantitative machine learning framework that processes 5-level bid/ask depth snapshots sampled at high resolution to model forward realized volatility.

Key components of the methodology:
- **Order Book Imbalance (OBI):** Multi-level volume imbalances capturing directional order flow pressure.
- **Cross-Asset Spillover:** Joint modeling of Bitcoin (BTC) and Solana (SOL) order books at matching timestamps to capture liquidity lead-lag dynamics.
- **Microstructural Noise Filtering:** Demonstration of early stopping dynamics on tree ensembles to prevent overfitting on tiny volatility targets (~$10^{-5}$).
- **Continuous Multi-Scale Volatility (EWMA):** Exponentially weighted moving average variance memory and normalized order book quote range proxies.
- **Pearson Information Coefficient (IC):** Evaluation via Pearson correlation ($r$) between forward realized volatility and predictions, measuring true directional ranking rather than deceptive scale-only metrics.

## Benchmark Results

Evaluated on a chronological 80/20 train/validation split (preserving temporal causality without lookahead bias):

| Stage / Model | Hyperparameters / Setup | Pearson Correlation ($r$) | Validation RMSE | Status |
| :--- | :--- | :--- | :--- | :--- |
| **Stage 4: Champion LightGBM** | Continuous L2 Depth, Multi-Scale EWMA, Range Proxies | **+0.6753** | **0.000074** | 🥇 Champion |
| **Stage 4: Champion XGBoost (CUDA)** | Continuous L2 Depth, Multi-Scale EWMA, Range Proxies | **+0.6675** | **0.000075** | 🥈 Strong GPU Benchmark |
| **Stage 3: Targeted Econometric** | Squared Returns ($r_t^2$), Absolute Returns, Optuna | **+0.6586** | 0.000076 | Competitive |
| **Stage 2: Feature Engineering (v2)** | Spreads, Returns, Realized Volatility, OBI | **+0.6441** | 0.000078 | Volatility Clustering |
| **Stage 1: Raw L2 Baseline (LGBM)** | Raw 5-Level Depth Snapshots | **+0.0947** | 0.000162 | Static LOB Baseline |
| **Stage 1: Raw L2 Baseline (RF)** | Raw 5-Level Depth Snapshots | **-0.0189** | 0.000386 | Baseline (Directional Failure) |

*Key finding:* Bitcoin ask price levels (`ask_price1_BTC`, `ask_price2_BTC`) and multi-scale volatility memory serve as decisive leading indicators for Ethereum micro-volatility, pushing directional correlation from $+0.0947 ightarrow +0.6753$.

## Repository Structure

```text
├── data/
│   ├── raw/                 # Raw L2 order book CSVs (train/test) [Git-ignored]
│   └── processed/           # Cached engineered feature matrices
├── docs/
│   ├── methodology.md       # Empirical design, Pearson IC & validation strategy
│   ├── features.md          # Mathematical feature formulas, OBI, EWMA volatility
│   └── data_dictionary.md   # Order book column definitions
├── models/                  # Serialized champion models and feature sets
├── notebooks/               # Exploratory Data Analysis & visual diagnostics
├── src/                     # Core pipeline source code
│   ├── stage1_train_baselines.py
│   ├── stage1_eval_baselines.py
│   ├── stage1_early_stopping.py
│   ├── stage1_overfitting_proof.py
│   ├── stage2_cross_asset_features.py
│   ├── stage2_realized_volatility.py
│   ├── stage2_volatility_clustering.py
│   ├── stage2_pca_vs_tree_reduction.py
│   ├── stage2_compare_feature_sets.py
│   ├── stage3_feature_selection_tuning.py
│   ├── stage3_advanced_tuning.py
│   ├── stage3_eval_all_models.py
│   ├── stage4_continuous_features_test.py
│   └── stage4_champion_pipeline.py
├── tests/                   # Data validation tests
├── .env.example             # Runtime configuration template
├── .gitignore               # Strict ignore rules for large datasets and caches
└── requirements.txt         # Pinned project dependencies
```

## Setup & Quickstart

### Prerequisites
Python 3.10+ (tested with Python 3.14 on Linux). CUDA-compatible GPU optional for accelerated XGBoost training.

```bash
# Clone the repository
git clone https://github.com/your-username/Stock-Market-Prediction.git
cd "Stock Market Prediction"

# Create and activate virtual environment
python -m venv .venv
source .venv/bin/activate

# Install dependencies
pip install -r requirements.txt
```

### Reproducing Benchmark Scores
To evaluate the baseline models:
```bash
python src/stage1_eval_baselines.py
```

To run the full champion multi-scale pipeline:
```bash
python src/stage4_champion_pipeline.py
```
