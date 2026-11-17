# 9. References and Appendices

## References
All bibliographic entries are formatted in standard IEEE citation style:

1. [1] T. Andersen and T. Bollerslev, "Answering the skeptics: Yes, standard volatility models do provide accurate forecasts," *International Economic Review*, vol. 39, no. 4, pp. 885–905, 1998.
2. [2] R. Engle, "Autoregressive conditional heteroscedasticity with estimates of the variance of United Kingdom inflation," *Econometrica*, vol. 50, no. 4, pp. 987–1007, 1982.
3. [3] T. Bollerslev, "Generalized autoregressive conditional heteroskedasticity," *Journal of Econometrics*, vol. 31, no. 3, pp. 307–327, 1986.
4. [4] B. Mandelbrot, "The variation of certain speculative prices," *The Journal of Business*, vol. 36, no. 4, pp. 394–419, 1963.
5. [5] R. Cont, A. Kukanov, and S. Stoikov, "The price impact of order book events," *Journal of Financial Econometrics*, vol. 12, no. 1, pp. 47–88, 2014.
6. [6] F. Corsi, "A simple approximate long-memory model of realized volatility," *Journal of Financial Econometrics*, vol. 7, no. 2, pp. 174–196, 2009.
7. [7] U. Müller, M. Dacorogna, R. Davé, O. Pictet, R. Olsen, and J. Ward, "Volatilities of different time resolutions—Analyzing the dynamics of market components," *Journal of Empirical Finance*, vol. 4, no. 2-3, pp. 213–239, 1997.
8. [8] M. Parkinson, "The extreme value method for estimating the variance of the rate of return," *The Journal of Business*, vol. 53, no. 1, pp. 61–65, 1980.
9. [9] J.P. Morgan, "RiskMetrics—Technical Document," 4th ed., J.P. Morgan / Reuters, New York, 1996.
10. [10] T. Chen and C. Guestrin, "XGBoost: A scalable tree boosting system," in *Proc. 22nd ACM SIGKDD Int. Conf. on Knowledge Discovery and Data Mining*, pp. 785–794, 2016.
11. [11] G. Ke, Q. Meng, T. Finley, T. Wang, W. Chen, W. Ma, Q. Ye, and T.-Y. Liu, "LightGBM: A highly efficient gradient boosting decision tree," in *Advances in Neural Information Processing Systems (NeurIPS)*, vol. 30, pp. 3146–3154, 2017.
12. [12] J. Bergstra, R. Bardenet, Y. Bengio, and B. Kégl, "Algorithms for hyper-parameter optimization," in *Advances in Neural Information Processing Systems (NeurIPS)*, vol. 24, pp. 2546–2554, 2011.
13. [13] T. Akiba, S. Sano, T. Yanase, T. Ohta, and M. Koyama, "Optuna: A next-generation hyperparameter optimization framework," in *Proc. 25th ACM SIGKDD Int. Conf. on Knowledge Discovery and Data Mining*, pp. 2623–2631, 2019.
14. [14] J. Bates and C. Granger, "The combination of forecasts," *Journal of the Operational Research Society*, vol. 20, no. 4, pp. 451–468, 1969.
15. [15] D. Wolpert, "Stacked generalization," *Neural Networks*, vol. 5, no. 2, pp. 241–259, 1992.
16. [16] A. Timmermann, "Forecast combinations," in *Handbook of Economic Forecasting*, vol. 1, pp. 135–196, Elsevier, 2006.
17. [17] A. Patton, "Volatility forecast comparison using imperfect volatility proxies," *Journal of Econometrics*, vol. 160, no. 1, pp. 246–256, 2011.
18. [18] F. Diebold and R. Mariano, "Comparing predictive accuracy," *Journal of Business & Economic Statistics*, vol. 13, no. 3, pp. 253–263, 1995.
19. [19] W. Newey and K. West, "A simple, positive semi-definite, heteroskedasticity and autocorrelation consistent covariance matrix," *Econometrica*, vol. 55, no. 3, pp. 703–708, 1987.

---

## Appendix A: Hyperparameter Specifications per Model
1. **Calibrated RidgeCV (30 Vol Features):**
   - Standardization: `StandardScaler(with_mean=True, with_std=True)`
   - Alpha grid: 15 log-spaced values from $10^{-4}$ to $10^{4}$ (`np.logspace(-4, 4, 15)`)
   - Cross-validation: Generalized Cross-Validation (GCV) on 424,156 training rows.
2. **LightGBM Conservative (Depth 4):**
   - `n_estimators=3000`, `max_depth=4`, `num_leaves=15`, `learning_rate=0.05`
   - `colsample_bytree=0.70`, `subsample=0.80`, `random_state=42`, `early_stopping=50`
3. **XGBoost Conservative (Depth 4):**
   - `n_estimators=3000`, `max_depth=4`, `learning_rate=0.05`, `colsample_bytree=0.60`
   - `subsample=0.80`, `tree_method='hist'`, `device='cuda'`, `early_stopping_rounds=50`
   - Serialized with `device='cpu'` for deployment.
4. **LightGBM Deep (Depth 6):**
   - `n_estimators=3000`, `max_depth=6`, `num_leaves=40`, `learning_rate=0.03`
   - `colsample_bytree=0.80`, `subsample=0.80`, `early_stopping=50`
5. **XGBoost Deep (Depth 6):**
   - `n_estimators=3000`, `max_depth=6`, `learning_rate=0.03`, `colsample_bytree=0.80`
   - `subsample=0.80`, `tree_method='hist'`, `device='cuda'`, `early_stopping_rounds=50`
6. **Optuna Stage 7 Tuned Models:**
   - LightGBM: `n_estimators=1200`, `max_depth=5`, `num_leaves=37`, `learning_rate=0.015`, `colsample_bytree=0.50`, `subsample=0.60`, `min_child_samples=50`
   - XGBoost: `n_estimators=2000`, `max_depth=4`, `learning_rate=0.035`, `colsample_bytree=0.50`, `subsample=0.75`, `min_child_weight=7`

---

## Appendix B: The Complete 135-Feature Space Listing
The complete feature listing exported to `models/final_feature_list.joblib` consists of:
- **ETH Base (21):** `mid_price`, `bid_price1-5`, `bid_volume1-5`, `ask_price1-5`, `ask_volume1-5`
- **BTC Base (21):** `mid_price_BTC`, `bid_price1-5_BTC`, `bid_volume1-5_BTC`, `ask_price1-5_BTC`, `ask_volume1-5_BTC`
- **SOL Base (21):** `mid_price_SOL`, `bid_price1-5_SOL`, `bid_volume1-5_SOL`, `ask_price1-5_SOL`, `ask_volume1-5_SOL`
- **ETH Engineered (21):** `log_return`, `ret_squared`, `abs_return`, `realized_vol_5`, `realized_vol_10`, `realized_vol_15`, `realized_vol_30`, `realized_vol_60`, `ewma_vol_10`, `ewma_vol_30`, `ewma_vol_60`, `spread1-5`, `obi1-3`, `norm_range_15`, `norm_range_30`
- **BTC Engineered (21):** `log_return_BTC`, `ret_squared_BTC`, `abs_return_BTC`, `realized_vol_5_BTC`, `realized_vol_10_BTC`, `realized_vol_15_BTC`, `realized_vol_30_BTC`, `realized_vol_60_BTC`, `ewma_vol_10_BTC`, `ewma_vol_30_BTC`, `ewma_vol_60_BTC`, `spread1-5_BTC`, `obi1-3_BTC`, `norm_range_15_BTC`, `norm_range_30_BTC`
- **SOL Engineered (21):** `log_return_SOL`, `ret_squared_SOL`, `abs_return_SOL`, `realized_vol_5_SOL`, `realized_vol_10_SOL`, `realized_vol_15_SOL`, `realized_vol_30_SOL`, `realized_vol_60_SOL`, `ewma_vol_10_SOL`, `ewma_vol_30_SOL`, `ewma_vol_60_SOL`, `spread1-5_SOL`, `obi1-3_SOL`, `norm_range_15_SOL`, `norm_range_30_SOL`
- **Cross-Asset & Lags (9):** `ret_lag1-3`, `rvol10_lag1-3`, `mid_ratio_ETH_BTC`, `mid_ratio_ETH_SOL`, `ret_diff_ETH_BTC`

---

## Appendix C: Stage, Script, and Execution Log Mapping
All empirical results are fully traceable to execution records in `logs/run_2026-11-09.log`:
- Raw Baselines: lines 12–148 (`src/stage1_train_baselines.py`)
- Overfitting Proof: lines 150–210 (`src/stage1_overfitting_proof.py`)
- Cross-Asset & Reduction: lines 212–390 (`src/stage2_*.py`)
- Continuous EWMA Pipeline: lines 420–580 (`src/stage4_multiscale_pipeline.py`)
- Ensembles & SLSQP: lines 582–720 (`src/stage5_model_ensemble.py`)
- Stacking Meta-Learners: lines 722–840 (`src/stage6_stacking_ensemble.py`)
- Optuna Blend: lines 842–950 (`src/stage7_optuna_ensemble.py`)
- Authoritative 3-Way Benchmark: lines 952–1077 (`src/eval_rigorous_test_benchmark.py`)

---

## Appendix D: Step-by-Step Reproduction Instructions
To reproduce all reported findings, figures, and artifacts from scratch:
```bash
# 1. Clone repository and activate Python virtual environment
cd /home/plyrX06996/Documents/GitHub/Secondary/TAI_project
source .venv/bin/activate

# 2. Run automated data and methodology integrity verification
python src/verify_data_integrity.py

# 3. Execute authoritative 3-way chronological benchmark with app export
python src/eval_rigorous_test_benchmark.py --export-app

# 4. Execute the automated pytest test suite
pytest -q tests/

# 5. Generate publication figures and verified facts.json
python src/make_figures.py

# 6. Build the 16:9 viva presentation deck (PPTX and PDF)
python presentation/build_deck.py

# 7. Compile the complete academic project report
python report/build_report.py

# 8. Launch the interactive Streamlit quantitative terminal
streamlit run app.py
```
