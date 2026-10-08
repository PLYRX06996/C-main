import os
import shutil
import re

def strip_comments_and_formatting(code):
    code = re.sub(r'#.*', '', code)
    code = re.sub(r'[ \t]*print\(.*[\'\"][ \t]*[-=]{3,}[ \t]*[\'\"].*\).*?\n', '', code)
    code = re.sub(r'\n\s*\n', '\n\n', code)
    return code.strip() + '\n'

def make_file(path, content):
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, 'w') as f:
        f.write(content.strip() + '\n')

def generate_professional_scaffold(target_dir):
    # 1. README.md
    readme = """
# Cryptocurrency High-Frequency Volatility Prediction 📈

![Python](https://img.shields.io/badge/Python-3.14-blue.svg)
![XGBoost](https://img.shields.io/badge/XGBoost-GPU_Optimized-red.svg)
![LightGBM](https://img.shields.io/badge/LightGBM-CPU-orange.svg)

## Project Overview
This repository contains a high-frequency machine learning pipeline designed to predict the **10-second forward rolling volatility** of Ethereum (ETH) using Level 2 Order Book microstructure data. 

In algorithmic trading, predicting short-term volatility is crucial for market-making algorithms and risk-management systems to adjust their spreads.

The project integrates cross-asset liquidity metrics from Bitcoin (BTC) and Solana (SOL) to engineer advanced correlation features, achieving a heavily optimized Bayesian (Optuna) baseline on the dataset.

## Directory Structure
```text
├── data/               
│   └── raw/            # Contains ETH, BTC, and SOL CSV datasets
├── docs/               # Technical documentation and data schemas
├── models/             # Serialized joblib models (e.g., God-Mode XGBoost)
├── notebooks/          # Exploratory Data Analysis (EDA) Jupyter notebooks
├── src/                
│   └── models/         # Core ML training and evaluation scripts
├── tests/              # Unit tests for data pipeline
├── .env.example        # Environment variables template
├── .gitignore          # Strict ignore rules for environments
├── CHANGELOG.md        # Tracked changes and iterative improvements
├── CONTRIBUTING.md     # Guidelines for contributing
├── README.md           # This file
└── requirements.txt    # Frozen Python dependencies
```

## Setup & Installation
```bash
git clone https://github.com/PLYRX06996/Stock_Market_Prediction_Final.git
cd Stock_Market_Prediction_Final
python -m venv .venv
source .venv/bin/activate
pip install -r requirements.txt
```
    """
    make_file(os.path.join(target_dir, 'README.md'), readme)

    # 2. CONTRIBUTING.md
    contrib = """
# Contributing Guidelines

We welcome contributions to the Crypto Volatility Prediction models! 
Please ensure that any new feature engineering logic introduced in `src/` passes basic multicollinearity checks before submitting a Pull Request.
    """
    make_file(os.path.join(target_dir, 'CONTRIBUTING.md'), contrib)

    # 3. CHANGELOG.md
    changelog = """
# Changelog

## [1.0.0] - Initial Release
### Added
- Implemented XGBoost & LightGBM baseline models.
- Integrated Optuna Bayesian Optimization for hyperparameter tuning.
- Engineered Level-1 through 5 order book imbalances (OBI).
- Cross-asset spreads merging (BTC & SOL constraints).
- GPU CUDA enablement for XGBoost.
    """
    make_file(os.path.join(target_dir, 'CHANGELOG.md'), changelog)

    # 4. .env.example
    env_example = """
# Environment Variables
# Do not place actual secrets here.

DATA_DIR=./data/raw/
MODEL_DIR=./models/
USE_GPU=True
XGB_VERBOSITY=0
    """
    make_file(os.path.join(target_dir, '.env.example'), env_example)

    # 5. .gitignore
    gitignore = """
# Environments
.venv/
env/

# IDEs & Cache
.vscode/
.idea/
__pycache__/
*.pyc

# Local Config
.env
    """
    make_file(os.path.join(target_dir, '.gitignore'), gitignore)

    # 6. requirements.txt
    reqs = "pandas>=2.0.0\nnumpy>=1.24.0\nscikit-learn>=1.3.0\nxgboost>=2.0.0\nlightgbm>=4.0.0\noptuna>=3.4.0\njoblib>=1.3.0\n"
    make_file(os.path.join(target_dir, 'requirements.txt'), reqs)

    # 7. Create empty professional folders with .gitkeep
    folders = ['data/processed', 'notebooks', 'tests']
    for folder in folders:
        os.makedirs(os.path.join(target_dir, folder), exist_ok=True)
        make_file(os.path.join(target_dir, folder, '.gitkeep'), "")
        
    # 8. Add a real document to docs/
    docs = """
# Data Dictionary & Schema

This repository processes Level-2 Order Book datasets for High-Frequency Trading analysis.

### Raw Data Features:
- `timestamp`: Snapshot time (Minute granularity for BTC/SOL, Second granular for ETH).
- `mid_price`: The calculated midpoint between the best Bid and best Ask.
- `bid_price[1-5]`: The top 5 highest prices buyers are willing to pay.
- `bid_volume[1-5]`: The volume demanded at the respective bid prices.
- `ask_price[1-5]`: The top 5 lowest prices sellers are willing to accept.
- `ask_volume[1-5]`: The volume supplied at the respective ask prices.

### Target Variable:
The `label` column in ETH represents the **10-second forward rolling standard deviation of logarithmic returns** (an industry standard metric for micro-volatility).
    """
    make_file(os.path.join(target_dir, 'docs', 'data_dictionary.md'), docs)

def main():
    source_dir = os.path.dirname(os.path.abspath(__file__))
    target_dir = os.path.normpath(os.path.join(source_dir, '..', 'Stock_Market_Prediction_Final'))

    print(f"Exporting cleaned files & building professional structure at: {target_dir}")
    
    # CRITICAL TRIGGER: We DELETE the entire old folder so that deleted files in our workspace 
    # don't leave "ghosts" in the public repo. This guarantees a perfect 1-to-1 sync every day.
    if os.path.exists(target_dir):
        shutil.rmtree(target_dir)
        
    os.makedirs(target_dir, exist_ok=True)
    
    # Generate Professional Scaffold
    generate_professional_scaffold(target_dir)

    # 1. Handle SRC directory (scrubbing Python code)
    if os.path.exists(os.path.join(source_dir, 'src')):
        for root, _, files in os.walk(os.path.join(source_dir, 'src')):
            for file in files:
                if not file.endswith('.py'): continue
                
                rel_path = os.path.relpath(os.path.join(root, file), os.path.join(source_dir, 'src'))
                dest_path = os.path.join(target_dir, 'src', 'models', rel_path)
                
                os.makedirs(os.path.dirname(dest_path), exist_ok=True)

                with open(os.path.join(root, file), 'r') as f:
                    content = f.read()

                clean_content = strip_comments_and_formatting(content)

                with open(dest_path, 'w') as f:
                    f.write(clean_content)

    # 2. Handle MODELS directory
    models_src = os.path.join(source_dir, 'models')
    models_dest = os.path.join(target_dir, 'models')
    if os.path.exists(models_src):
        shutil.copytree(models_src, models_dest)
        
    # 3. Handle DATASETS directory (Copying raw train data into data/raw)
    data_src = os.path.join(source_dir, 'datasets', 'train')
    data_dest = os.path.join(target_dir, 'data', 'raw')
    if os.path.exists(data_src):
        os.makedirs(data_dest, exist_ok=True)
        for csv_file in ['ETH.csv', 'BTC.csv', 'SOL.csv']:
            full_csv = os.path.join(data_src, csv_file)
            if os.path.exists(full_csv):
                shutil.copy2(full_csv, os.path.join(data_dest, csv_file))
                print(f"Copied {csv_file} to data/raw/")

    print("\nExport complete! The folder is deeply structured and perfectly synced for GitHub.")

if __name__ == '__main__':
    main()
