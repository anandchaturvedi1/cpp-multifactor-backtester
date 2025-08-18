# Multi-Factor Long/Short Equity Backtester (C++)

<img width="700" height="450" alt="cumulative_returns" src="https://github.com/user-attachments/assets/344be388-9350-4b77-bd6e-8e164c9934d5" />

## About this Project
This repository contains a **C++-based multi-factor equity backtester** designed to evaluate long/short portfolio strategies using momentum and volatility signals. Built as part of my ongoing work in quantitative research and trading strategy development, it demonstrates my ability to:

- Implement finance-relevant algorithms in **performance-oriented C++**
- Structure code for scalability, factor extensibility, and fast computation
- Combine **quantitative finance concepts** with **robust software engineering**

While much of my professional quant work has been in **Python, R, and kdb+/q**, this project showcases how I can translate research logic into **production-style C++ applications** — a key skill for hedge funds, prop shops, and systematic trading teams.

---

## Strategy Overview

**Factor Model:**
- Momentum = % change over a rolling lookback window
- Volatility = Standard deviation of daily returns (optional in current config)
- Extendable to value, quality, and other alpha signals

**Portfolio Construction:**
- Rank universe by factor score
- Go **long** on top *N* names, **short** on bottom *N*
- Equal-weight allocation within long and short baskets
- Rebalance every *X* trading days

**Performance Metrics:**
- **Total Return**
- **Annualized Sharpe Ratio**
- **Maximum Drawdown**

---

## Running the Backtester
### Compile & Execute C++ Backtester
```bash
g++ src/main.cpp src/data_loader.cpp src/factors.cpp src/backtester.cpp -o backtester
./backtester
```

### Backtest Results

| Metric            | Value    |
|-------------------|----------|
| Total Return      | 4348.20% |
| Annualized Sharpe | 0.95     |
| Max Drawdown      | 64.45%   |

