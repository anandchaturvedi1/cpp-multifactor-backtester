# Multi-Factor Long/Short Equity Backtester (C++)

## About this Project
This repository contains a **C++-based multi-factor equity backtester** designed to evaluate long/short portfolio strategies using momentum and volatility signals. Built as part of my ongoing work in quantitative research and trading strategy development, it demonstrates my ability to:

- **Implement finance-relevant algorithms in performance-oriented C++.**
- **Structure code for scalability, factor extensibility, and fast computation.**
- **Combine quantitative finance concepts with robust software engineering.**

While much of my professional quant work has been in **Python, R, and kdb+/q**, this project showcases how I can translate research logic into **production-style C++ applications** — a key skill for hedge funds, prop shops, and systematic trading teams that require both research insight and low-latency implementation.

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

