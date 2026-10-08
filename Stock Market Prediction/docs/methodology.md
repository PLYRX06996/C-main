# Empirical Methodology

## 1. Temporal Integrity & Validation Design
Because high-frequency financial time series exhibit strong autocorrelation and non-stationarity, standard K-Fold cross validation causes lookahead data leakage. All evaluations use a chronological train/validation split (`shuffle=False`, 80% train, 20% validation).

## 2. Early Stopping & Noise Overfitting
Because the target volatility magnitude is small ($\sim 10^{-5}$), decision tree ensembles face immediate risk of fitting idiosyncratic noise. 

Experiments demonstrated that unconstrained boosting (e.g. 1,000 trees) degrades validation RMSE significantly compared to early-stopped models (stopping around 15–50 iterations with learning rate $\eta = 0.05$ to $0.09$).

## 3. Bayesian Optimization
Hyperparameters were tuned using Optuna's Tree-structured Parzen Estimator (TPE), searching:
- `max_depth`: [3, 7]
- `learning_rate`: [0.01, 0.15]
- `subsample`: [0.6, 1.0]
- `colsample_bytree`: [0.5, 1.0]
- `min_child_weight`: [1, 10]
