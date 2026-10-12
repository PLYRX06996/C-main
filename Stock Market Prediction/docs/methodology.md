# Empirical Methodology

## 1. Temporal Integrity & Validation Design
Because high-frequency financial time series exhibit strong autocorrelation and non-stationarity, standard K-Fold cross validation causes lookahead data leakage. All evaluations use a chronological train/validation split (`shuffle=False`, 80% train, 20% validation).

## 2. Evaluation Metrics: Pearson Correlation ($r$) vs. Scale Losses
In high-frequency markets where forward volatility magnitude is minuscule ($\\sim 10^{-5}$), standard scale metrics like MSE or RMSE can be deceptively small even for naive mean-predicting baselines.
To evaluate true financial ranking and directional forecasting capability, we benchmark models on the **Pearson Information Coefficient (IC)**:
$$r = \frac{\sum (y_i - \bar{y})(\hat{y}_i - \bar{\hat{y}})}{\sqrt{\sum (y_i - \bar{y})^2 \sum (\hat{y}_i - \bar{\hat{y}})^2}}$$
A model with high positive Pearson correlation reliably predicts volatility expansion and contraction regimes.

## 3. Early Stopping & Noise Overfitting
Because the target volatility magnitude is small ($\\sim 10^{-5}$), decision tree ensembles face immediate risk of fitting idiosyncratic noise. 

Experiments demonstrated that unconstrained boosting (e.g. 1,000 trees) degrades validation RMSE and collapses out-of-sample directional correlation compared to early-stopped models (stopping around 15–50 iterations with learning rate $\eta = 0.04$ to $0.09$).

## 4. Bayesian Optimization
Hyperparameters were tuned using Optuna's Tree-structured Parzen Estimator (TPE), searching:
- `max_depth`: [3, 8]
- `learning_rate`: [0.01, 0.15]
- `subsample`: [0.5, 1.0]
- `colsample_bytree`: [0.4, 1.0]
- `num_leaves`: [15, 63]
- `min_child_samples`: [10, 100]
