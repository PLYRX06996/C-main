# Cryptocurrency Limit Order Book Volatility Prediction

An empirical research pipeline predicting 10-second forward rolling volatility of Ethereum (ETH) using high-frequency Level-2 Limit Order Book (LOB) microstructure and cross-asset signals (BTC, SOL).

## Project Overview

In high-frequency cryptocurrency markets, short-horizon volatility forecasting is critical for market making, risk management, and algorithmic execution. This repository implements an end-to-end machine learning framework that processes 5-level bid/ask depth snapshots sampled at high resolution to model forward realized volatility.

Key components of the methodology:
- **Order Book Imbalance (OBI):** Multi-level volume imbalances capturing directional order flow pressure.
- **Cross-Asset Spillover:** Joint modeling of Bitcoin (BTC) and Solana (SOL) order books at matching timestamps to capture liquidity lead-lag dynamics.
- **Microstructural Noise Filtering:** Demonstration of early stopping dynamics on tree ensembles to prevent overfitting on tiny volatility targets (~$10^{-5}$).
- **Bayesian Optimization:** Automated hyperparameter tuning and feature selection using Tree-structured Parzen Estimator (TPE) via Optuna.

## Benchmark Results

Evaluated on a chronological 80/20 train/validation split (preserving temporal causality without lookahead bias):

| Model | Hyperparameters / Setup | Validation RMSE | Status |
| :--- | :--- | :--- | :--- |
| **XGBoost (Hist/CUDA)** | 25 Selected Features, Bayesian Tuned | **0.00007876** | Champion |
| **LightGBM** | 25 Selected Features, Early Stopping | **0.00008427** | Competitive |
| **Random Forest** | 150 Trees, Max Depth 8 | **0.00061858** | Baseline |

*Key finding:* Bitcoin ask price levels (`ask_price1_BTC`, `ask_price2_BTC`) serve as strong leading indicators for Ethereum micro-volatility, whereas Solana order book features contributed negligible marginal predictive value.

## Repository Structure

```text
├── data/
│   ├── raw/                 # Raw L2 order book CSVs (train/test) [Git-ignored]
│   └── processed/           # Cached engineered feature matrices
├── docs/
│   ├── methodology.md       # Empirical design and validation strategy
│   ├── features.md          # Mathematical feature formulas and OBI
│   └── data_dictionary.md   # Order book column definitions
├── models/                  # Serialized champion models and feature sets
├── notebooks/               # Exploratory Data Analysis & visual diagnostics
├── src/                     # Core pipeline source code
│   ├── baseline_models.py
│   ├── early_stop_models.py
│   ├── force_1000.py
│   ├── feature_engineering.py
│   ├── feature_selection_tuning.py
│   ├── advanced_tuning.py
│   └── final_evaluation_all.py
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
To run the full evaluation across all tuned models:
```bash
python src/final_evaluation_all.py
```
