#ifndef FACTORS_H
#define FACTORS_H

#include "data_loader.h"
#include <vector>
#include <string>
#include <map>

// Calculates momentum: % change between price today and price N days ago
double calcMomentum(const std::vector<double>& prices, int lookbackDays);

// Calculates volatility: standard deviation of daily returns over N days
double calcVolatility(const std::vector<double>& prices, int lookbackDays);

// Extracts closing prices for a specific ticker from dataset
std::vector<double> getClosingPrices(const std::vector<StockData>& data, const std::string& ticker);

#endif
