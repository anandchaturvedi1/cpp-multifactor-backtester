# Multi-Factor Long/Short Equity Backtester (C++)

This project implements a simple **multi-factor equity long/short strategy backtester** in **C++**, designed for performance and flexibility.  
It calculates factors, ranks stocks, builds portfolios, and evaluates strategy performance.

---

## Features
- Loads historical price data from CSV
- Calculates momentum (and extendable to volatility, value, etc.)
- Ranks stocks into **long** and **short** baskets
- Equal-weight portfolio simulation with periodic rebalancing
- Computes:
  - Cumulative returns
  - Annualized Sharpe ratio
  - Maximum drawdown
- Outputs results to CSV for further analysis/plotting in Python

---

## Strategy Logic
1. **Factor Calculation:**  
   - Momentum = % change over a specified lookback period
   - Extendable to volatility, value, quality, etc.
2. **Portfolio Construction:**  
   - Top N ranked stocks → Longs
   - Bottom N ranked stocks → Shorts
3. **Backtest:**  
   - Rebalance every X days
   - Calculate next-day returns for long and short baskets
4. **Performance Metrics:**  
   - Total Return
   - Annualized Sharpe
   - Max Drawdown

---

## How to Run
### Compile & Execute C++ Backtester
```bash
g++ src/main.cpp src/data_loader.cpp src/factors.cpp src/backtester.cpp -o backtester
./backtester
```

## Results

The backtest generates a performance plot of cumulative returns:

<p align="center">
  <img width="700" height="450" alt="cumulative_returns" src="https://github.com/user-attachments/assets/344be388-9350-4b77-bd6e-8e164c9934d5" />
</p>

### Key Metrics

| Metric            | Value    |
|-------------------|----------|
| Total Return      | 4348.20% |
| Annualized Sharpe | 0.95     |
| Max Drawdown      | 64.45%   |

