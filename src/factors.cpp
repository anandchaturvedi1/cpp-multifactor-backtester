#include "../include/factors.h"
#include <cmath>
#include <numeric>
#include <iostream>

double calcMomentum(const std::vector<double>& prices, int lookbackDays) {
    if (prices.size() <= lookbackDays || lookbackDays <= 0) return 0.0;
    double pastPrice = prices[prices.size() - lookbackDays - 1];
    double latestPrice = prices.back();
    return ((latestPrice - pastPrice) / pastPrice) * 100.0;
}

double calcVolatility(const std::vector<double>& prices, int lookbackDays) {
    if (prices.size() <= lookbackDays || lookbackDays <= 1) return 0.0;

    std::vector<double> returns;
    for (size_t i = prices.size() - lookbackDays; i < prices.size(); ++i) {
        if (i == 0) continue;
        double dailyReturn = (prices[i] - prices[i - 1]) / prices[i - 1];
        returns.push_back(dailyReturn);
    }

    double mean = std::accumulate(returns.begin(), returns.end(), 0.0) / returns.size();
    double sq_sum = 0.0;
    for (double r : returns) {
        sq_sum += (r - mean) * (r - mean);
    }

    return std::sqrt(sq_sum / returns.size());
}

std::vector<double> getClosingPrices(const std::vector<StockData>& data, const std::string& ticker) {
    std::vector<double> closes;
    for (const auto& row : data) {
        if (row.ticker == ticker) {
            closes.push_back(row.close);
        }
    }
    return closes;
}
