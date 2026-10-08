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
# Cryptocurrency Order Book Volatility Prediction 📈

![Python](https://img.shields.io/badge/Python-3.14-blue.svg)
![XGBoost](https://img.shields.io/badge/XGBoost-GPU_Optimized-red.svg)
![License](https://img.shields.io/badge/License-MIT-green.svg)

## Project Overview
This repository contains a high-frequency machine learning pipeline designed to predict the **10-second forward rolling volatility** of Ethereum (ETH) using Level 2 Order Book microstructure data.

The project integrates cross-asset liquidity metrics from Bitcoin (BTC) and Solana (SOL) to engineer advanced correlation features, achieving a heavily optimized Bayesian (Optuna) baseline.

## Directory Structure
```text
├── data/               # Raw and processed datasets (Ignored in Git)
├── docs/               # Technical documentation and architecture
├── models/             # Serialized joblib models/weights
├── notebooks/          # Exploratory Data Analysis (EDA) Jupyter notebooks
├── src/                # Core ML pipeline source code
│   ├── features/       # Feature engineering modules
│   └── models/         # Training and evaluation scripts
├── tests/              # Unit tests for data pipeline
├── .env.example        # Environment variables template
├── .gitignore          # Strict ignore rules for datasets/environments
├── CHANGELOG.md        # Tracked changes and iterative improvements
├── CONTRIBUTING.md     # Guidelines for contributing
├── LICENSE             # MIT License
├── README.md           # This file
└── requirements.txt    # Frozen Python dependencies
```

## Setup & Installation
```bash
git clone https://github.com/yourusername/Stock-Market-Prediction.git
cd Stock-Market-Prediction
python -m venv .venv
source .venv/bin/activate
pip install -r requirements.txt
```
    """
    make_file(os.path.join(target_dir, 'README.md'), readme)

    # 2. LICENSE
    license_text = """
MIT License

Copyright (c) 2026

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction...
    """
    make_file(os.path.join(target_dir, 'LICENSE'), license_text)

    # 3. CONTRIBUTING.md
    contrib = """
# Contributing Guidelines

We welcome contributions to the Crypto Volatility Prediction models! 
Please ensure that any new feature engineering logic introduced in `src/` passes basic multicollinearity checks before submitting a Pull Request.
    """
    make_file(os.path.join(target_dir, 'CONTRIBUTING.md'), contrib)

    # 4. CHANGELOG.md
    changelog = """
# Changelog

## [1.0.0] - 2026-10-09
### Added
- Implemented XGBoost & LightGBM baseline models.
- Integrated Optuna Bayesian Optimization for hyperparameter tuning.
- Engineered Level-1 through 5 order book imbalances (OBI).
- Cross-asset spreads merging (BTC & SOL constraints).
- GPU CUDA enablement for XGBoost.
    """
    make_file(os.path.join(target_dir, 'CHANGELOG.md'), changelog)

    # 5. .env.example
    env_example = """
# Environment Variables
# Do not place actual secrets here.

DATA_DIR=./data/raw/
MODEL_DIR=./models/
USE_GPU=True
XGB_VERBOSITY=0
    """
    make_file(os.path.join(target_dir, '.env.example'), env_example)

    # 6. .gitignore
    gitignore = """
# Environments
.venv/
env/

# Datasets & Weights
data/
*.csv
*.pt
*.h5

# IDEs & Cache
.vscode/
.idea/
__pycache__/
*.pyc

# Local Config
.env
    """
    make_file(os.path.join(target_dir, '.gitignore'), gitignore)

    # 7. requirements.txt
    reqs = "pandas>=2.0.0\nnumpy>=1.24.0\nscikit-learn>=1.3.0\nxgboost>=2.0.0\nlightgbm>=4.0.0\noptuna>=3.4.0\njoblib>=1.3.0\n"
    make_file(os.path.join(target_dir, 'requirements.txt'), reqs)

    # 8. Create empty professional folders with .gitkeep
    folders = ['data/raw', 'data/processed', 'notebooks', 'tests', 'docs']
    for folder in folders:
        os.makedirs(os.path.join(target_dir, folder), exist_ok=True)
        make_file(os.path.join(target_dir, folder, '.gitkeep'), "")

def main():
    source_dir = os.path.dirname(os.path.abspath(__file__))
    target_dir = os.path.normpath(os.path.join(source_dir, '..', 'Stock_Market_Prediction_Final'))

    print(f"Exporting cleaned files & building professional structure at: {target_dir}")
    os.makedirs(target_dir, exist_ok=True)
    
    # Generate Professional Scaffold
    generate_professional_scaffold(target_dir)

    # 1. Handle SRC directory (scrubbing Python code)
    if os.path.exists(os.path.join(source_dir, 'src')):
        for root, _, files in os.walk(os.path.join(source_dir, 'src')):
            for file in files:
                if not file.endswith('.py'): continue
                
                # Split src items into models vs features inside target
                rel_path = os.path.relpath(os.path.join(root, file), os.path.join(source_dir, 'src'))
                dest_path = os.path.join(target_dir, 'src', 'models', rel_path) # Put our scripts in src/models/
                
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
        if os.path.exists(models_dest):
            shutil.rmtree(models_dest)
        shutil.copytree(models_src, models_dest)

    print("\nExport complete! The folder is deeply structured and ready for GitHub.")

if __name__ == '__main__':
    main()
