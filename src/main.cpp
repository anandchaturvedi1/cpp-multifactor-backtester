#include "../include/data_loader.h"
#include "../include/factors.h"
#include "../include/backtester.h"
#include <iostream>
#include <filesystem>

int main() {
    std::string filename = "../data/sample_data.csv";
    std::vector<StockData> marketData = loadCSV(filename);

    if (marketData.empty()) {
        std::cout << "No data loaded. Check file path.\n";
        return 1;
    }

    BacktestParams params;
    params.lookbackDays   = 60;  // momentum lookback
    params.topN           = 1;   // longs
    params.bottomN        = 1;   // shorts
    params.rebalanceEvery = 181;   // rebalance every 5 trading days

    auto res = runBacktest(marketData, params);

    double sharpe = computeAnnualizedSharpe(res);
    double maxDD  = computeMaxDrawdown(res);
    double totalReturn = (res.empty()? 0.0 : (res.back().cumReturn - 1.0));

    std::filesystem::create_directories("../out");
    bool ok = writeResultsCSV("../out/results.csv", res);

    std::cout << "Backtest complete.\n";
    std::cout << "Periods: " << res.size() << "\n";
    std::cout << "Total Return: " << totalReturn * 100.0 << "%\n";
    std::cout << "Annualized Sharpe: " << sharpe << "\n";
    std::cout << "Max Drawdown: " << maxDD * 100.0 << "%\n";
    std::cout << "Results CSV: " << (ok ? "../out/results.csv" : "write failed") << "\n";

    return 0;
}
