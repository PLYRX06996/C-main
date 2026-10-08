# Feature Engineering Documentation

## 1. Multi-Level Order Book Imbalance (OBI)
Order Book Imbalance measures the asymmetry between buying and selling interest across level $k$:
$$\text{OBI}_k = \frac{\text{bid\_volume}_k - \text{ask\_volume}_k}{\text{bid\_volume}_k + \text{ask\_volume}_k}$$

Calculated across levels 1 through 5 for ETH, BTC, and SOL.

## 2. Bid-Ask Spread Metrics
- **Absolute Spread:**
  $$\text{Spread} = \text{ask\_price1} - \text{bid\_price1}$$
- **Relative Spread:**
  $$\text{Relative\_Spread} = \frac{\text{Spread}}{\text{mid\_price}}$$

## 3. Cross-Asset Dynamics
- Mid-price ratios (ETH / BTC, ETH / SOL).
- Relative liquidity depth comparisons across simultaneous books.
