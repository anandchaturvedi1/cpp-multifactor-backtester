#ifndef BACKTESTER_H
#define BACKTESTER_H

#include "data_loader.h"
#include <string>
#include <vector>
#include <map>
#include <set>

struct DailyResult {
    std::string date;
    double longReturn;     // avg return of long basket
    double shortReturn;    // avg return of short basket
    double longShort;      // longReturn - shortReturn
    double cumReturn;      // compounded cumulative return (1+x)
};

struct BacktestParams {
    int lookbackDays = 20;     // rolling lookback for factor calc
    int topN = 5;              // longs
    int bottomN = 5;           // shorts
    int rebalanceEvery = 5;    // days between rebalances
};

std::vector<DailyResult> runBacktest(
    const std::vector<StockData>& data,
    const BacktestParams& params);

// Metrics
double computeAnnualizedSharpe(const std::vector<DailyResult>& res, int tradingDays=252);
double computeMaxDrawdown(const std::vector<DailyResult>& res);

// IO
bool writeResultsCSV(const std::string& path, const std::vector<DailyResult>& res);

#endif
