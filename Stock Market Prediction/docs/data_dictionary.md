# Data Dictionary & Schema Specification

The dataset contains millisecond/second-level Level-2 order book snapshots for three major crypto pairs (ETH, BTC, SOL) captured concurrently.

### Order Book Columns (Per Asset)
- `timestamp`: Timestamp of the order book snapshot.
- `mid_price`: Arithmetic midpoint between top bid and top ask:
  $$\text{mid\_price} = \frac{\text{bid\_price1} + \text{ask\_price1}}{2}$$
- `bid_price1` to `bid_price5`: Prices at the top 5 bid levels (descending order).
- `bid_volume1` to `bid_volume5`: Liquidity volume queued at each bid level.
- `ask_price1` to `ask_price5`: Prices at the top 5 ask levels (ascending order).
- `ask_volume1` to `ask_volume5`: Liquidity volume queued at each ask level.

### Target Variable
- `label` (present in `train/ETH.csv`):
  10-second forward rolling realized volatility, defined as the rolling sample standard deviation of log returns:
  $$\sigma_{10s} = \sqrt{\frac{1}{N-1} \sum_{i=1}^N (r_i - \bar{r})^2}$$
  where $r_i = \ln(P_i / P_{i-1})$.
