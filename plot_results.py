import pandas as pd
import matplotlib.pyplot as plt

# Load backtest results
df = pd.read_csv("out/results.csv")
df['date'] = pd.to_datetime(df['date'])

# Plot cumulative returns
plt.figure(figsize=(10,6))
plt.plot(df['date'], df['cum_return'], label="Long/Short Cumulative Return", color='blue')
plt.axhline(1.0, color='black', linewidth=0.8, linestyle='--')
plt.xlabel("Date")
plt.ylabel("Portfolio Value (Initial = 1.0)")
plt.title("Multi-Factor Long/Short Strategy Backtest (C++)")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig("out/cumulative_returns.png", dpi=300)
plt.show()

# Optional: Summary stats
total_return = df['cum_return'].iloc[-1] - 1
daily_returns = df['long_short']
sharpe = (daily_returns.mean() / daily_returns.std()) * (252 ** 0.5)
max_dd = ((df['cum_return'].cummax() - df['cum_return']) / df['cum_return'].cummax()).max()

print(f"Total Return: {total_return*100:.2f}%")
print(f"Annualized Sharpe: {sharpe:.2f}")
print(f"Max Drawdown: {max_dd*100:.2f}%")
