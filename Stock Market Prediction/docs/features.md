# Feature Engineering Documentation

The feature engineering pipeline transforms raw Level-2 Limit Order Book (L2 LOB) depth into a 135-column feature matrix capturing multi-scale volatility persistence, order flow pressure, and liquidity dispersion.

### Exact Feature Inventory (135 Columns)
| Feature Group | Column Count | Mathematical Formulation |
| :--- | :---: | :--- |
| **Raw LOB Depths (Per Asset $\times$ 3)** | 63 | Top 5 bid/ask prices and volumes + mid-price for ETH, BTC, SOL |
| **Microstructure Volatility (Per Asset $\times$ 3)** | 63 | Log returns, $r^2$, $|r|$, multi-horizon realized vol, EWMA vol, spreads 1–5, OBI 1–3, norm range 15/30 |
| **Cross-Asset & Autoregressive Lags** | 9 | `ret_lag1..3`, `rvol10_lag1..3`, mid-ratios (ETH/BTC, ETH/SOL), return diff (ETH - BTC) |
| **Total Features** | **135** | |

---

## 1. Multi-Scale Volatility Memory
- **Instantaneous Variance Shock:**
  $$r_t^2 = \left(\ln\frac{P_t}{P_{t-1}}\right)^2, \quad \text{abs\_return}_t = |r_t|$$
- **Multi-Horizon Rolling Realized Volatility:**
  $$\sigma_t^{(w)} = \sqrt{\frac{1}{w-1}\sum_{i=0}^{w-1} (r_{t-i} - \bar{r})^2}, \quad w \in \{5, 10, 15, 30, 60\}$$
  Computed with `min_periods=2` and causal forward filling to prevent cascading NaN data loss.
- **Exponentially Weighted Moving Average (EWMA) Volatility:**
  $$\sigma_{t,\text{EWMA}}^2 = \lambda \sigma_{t-1}^2 + (1-\lambda) r_t^2$$
  Spans $S \in \{10, 30, 60\}$ corresponding to decay parameters:
  - Span 10: $\lambda = 1 - 2/(10+1) \approx 0.818$
  - Span 30: $\lambda = 1 - 2/(30+1) \approx 0.935$
  - Span 60: $\lambda = 1 - 2/(60+1) \approx 0.967$

## 2. Order Book Imbalance (OBI)
Captures queued liquidity asymmetry across depth levels 1 to 3:
$$\text{OBI}_k = \frac{\text{bid\_volume}_k - \text{ask\_volume}_k}{\text{bid\_volume}_k + \text{ask\_volume}_k + 10^{-8}}, \quad k \in \{1, 2, 3\}$$

## 3. Bid-Ask Spreads & Range Proxies
- **Level-k Spreads:**
  $$\text{Spread}_k = \text{ask\_price}_k - \text{bid\_price}_k, \quad k \in \{1, 2, 3, 4, 5\}$$
- **Normalized Order Book Range Proxy:**
  $$\text{Range}_w = \frac{\max_{t-w:t}(\text{ask\_price5}) - \min_{t-w:t}(\text{bid\_price5})}{\text{mid\_price}_t + 10^{-8}}, \quad w \in \{15, 30\}$$

## 4. Market Feed Sanitization & Subsets
- **Price Sanitization:** Any sensor anomalies where price $\le 0$ or price $> 10^6$ are set to `NaN` and forward-filled before return computation.
- **30-Volatility Feature Matrix:** Contains strictly scale-free volatility estimators across ETH, BTC, and SOL (`realized_vol_*`, `ewma_vol_*`, `ret_squared_*`, `abs_return_*`). Used by linear regularized HAR models.
- **102-Stationary Feature Matrix:** Drops all 33 non-stationary raw dollar price levels (`mid_price`, `bid_price1..5`, `ask_price1..5`), retaining strictly stationary returns, spreads, imbalances, and volatilities.
