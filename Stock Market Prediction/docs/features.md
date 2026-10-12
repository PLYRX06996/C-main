# Feature Engineering Documentation

## 1. Volatility Clustering (ARCH/GARCH Formulations)
- **Instantaneous Return Variance Proxy:**
  $$r_t^2 = \left(\ln\frac{P_t}{P_{t-1}}\right)^2$$
- **Absolute Return Magnitude:**
  $$\text{abs\_return} = |r_t|$$
- **Exponentially Weighted Moving Average (EWMA) Volatility:**
  $$\sigma_{t,\text{EWMA}}^2 = \lambda \sigma_{t-1}^2 + (1-\lambda) r_t^2$$
  Evaluated across spans of 10, 30, and 60 ticks to model rapid variance shock memory.

## 2. Multi-Level Order Book Imbalance (OBI)
Order Book Imbalance measures the asymmetry between buying and selling interest across level $k$:
$$\text{OBI}_k = \frac{\text{bid\_volume}_k - \text{ask\_volume}_k}{\text{bid\_volume}_k + \text{ask\_volume}_k}$$

Calculated across levels 1 through 3 for ETH, BTC, and SOL.

## 3. Bid-Ask Spread & Range Metrics
- **Absolute Spread:**
  $$\text{Spread}_k = \text{ask\_price}_k - \text{bid\_price}_k$$
- **Normalized Order Book Range Proxy:**
  $$\text{Range}_w = \frac{\max_{t-w:t}(\text{ask\_price5}) - \min_{t-w:t}(\text{bid\_price5})}{\text{mid\_price}_t}$$
  Captures the rolling physical depth boundary expansion across recent tick history.

## 4. Cross-Asset Dynamics
- Mid-price ratios (ETH / BTC, ETH / SOL).
- Return difference / dispersion signals across simultaneous order book snapshots.
