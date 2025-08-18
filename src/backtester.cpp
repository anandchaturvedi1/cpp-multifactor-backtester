#include "../include/backtester.h"
#include "../include/factors.h"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <numeric>
#include <unordered_map>
#include <iostream>

// Helper: sort unique dates ascending
static std::vector<std::string> collectSortedDates(const std::vector<StockData>& data) {
    std::set<std::string> ds;
    for (const auto& r : data) ds.insert(r.date);
    return std::vector<std::string>(ds.begin(), ds.end());
}

// Build map[ticker] -> vector of (date, close) in chronological order
static std::unordered_map<std::string, std::vector<std::pair<std::string,double>>>
buildPriceSeries(const std::vector<StockData>& data) {
    std::unordered_map<std::string, std::vector<StockData>> tmp;
    for (const auto& r : data) tmp[r.ticker].push_back(r);
    std::unordered_map<std::string, std::vector<std::pair<std::string,double>>> series;
    for (auto& kv : tmp) {
        auto& v = kv.second;
        std::sort(v.begin(), v.end(), [](const StockData& a, const StockData& b){ return a.date < b.date; });
        std::vector<std::pair<std::string,double>> s; s.reserve(v.size());
        for (auto& r : v) s.emplace_back(r.date, r.close);
        series[kv.first] = std::move(s);
    }
    return series;
}

// Return index in series for given date, or -1 if not found
static int findIndexOnDate(const std::vector<std::pair<std::string,double>>& ser, const std::string& date) {
    int lo = 0, hi = (int)ser.size()-1;
    while (lo <= hi) {
        int mid = (lo+hi)/2;
        if (ser[mid].first == date) return mid;
        if (ser[mid].first < date) lo = mid+1; else hi = mid-1;
    }
    return -1;
}

// Compute momentum over lookback ending on index idx (inclusive)
static bool momentumAt(const std::vector<std::pair<std::string,double>>& ser, int idx, int lookback, double& out) {
    if (idx - lookback < 0) return false;
    double past = ser[idx - lookback].second;
    double latest = ser[idx].second;
    if (past == 0.0) return false;
    out = (latest - past) / past; // fraction (not %)
    return true;
}

// One-day forward return between idx and idx+1
static bool fwdReturnAt(const std::vector<std::pair<std::string,double>>& ser, int idx, double& out) {
    if (idx+1 >= (int)ser.size()) return false;
    double p0 = ser[idx].second;
    double p1 = ser[idx+1].second;
    if (p0 == 0.0) return false;
    out = (p1 - p0) / p0;
    return true;
}
std::vector<DailyResult> runBacktest(
    const std::vector<StockData>& data,
    const BacktestParams& params)
{
    // Prep
    auto dates = collectSortedDates(data);
    auto series = buildPriceSeries(data);
    // For rebalancing
    std::set<std::string> universe;
    for (const auto& kv : series) universe.insert(kv.first);

    std::vector<DailyResult> results;
    results.reserve(dates.size());

    std::set<std::string> currentLongs, currentShorts;
    int lastRebalanceIdx = -100000;

    // Iterate through dates; at each rebalance day, recompute factor ranks using data up to today
    for (size_t di = 0; di + 1 < dates.size(); ++di) { // stop at penultimate date (we need next-day return)
        const std::string& d = dates[di];
        bool doRebalance = ( (int)di - lastRebalanceIdx >= params.rebalanceEvery );

        if (doRebalance) {
            // Compute momentum for each ticker at date d
            std::vector<std::pair<std::string,double>> scores; scores.reserve(universe.size());
            for (const auto& t : universe) {
                auto it = series.find(t);
                if (it == series.end()) continue;
                int idx = findIndexOnDate(it->second, d);
                if (idx < 0) continue;
                double mom = 0.0;
                if (momentumAt(it->second, idx, params.lookbackDays, mom)) {
                    scores.emplace_back(t, mom);
                }
            }
            // Need enough names
            if ((int)scores.size() >= params.topN + params.bottomN) {
                std::sort(scores.begin(), scores.end(),
                          [](auto& a, auto& b){ return a.second < b.second; });
                currentShorts.clear();
                currentLongs.clear();
                for (int i = 0; i < params.bottomN; ++i) currentShorts.insert(scores[i].first);
                for (int i = 0; i < params.topN; ++i) currentLongs.insert(scores[(int)scores.size()-1 - i].first);
                lastRebalanceIdx = (int)di;
            }
        }

        // Compute next-day returns for baskets (from date d to d+1)
        double longSum = 0.0, shortSum = 0.0;
        int longCount = 0, shortCount = 0;

        for (const auto& t : currentLongs) {
            auto& ser = series[t];
            int idx = findIndexOnDate(ser, d);
            if (idx < 0) continue;
            double r;
            if (fwdReturnAt(ser, idx, r)) { longSum += r; longCount++; }
        }
        for (const auto& t : currentShorts) {
            auto& ser = series[t];
            int idx = findIndexOnDate(ser, d);
            if (idx < 0) continue;
            double r;
            if (fwdReturnAt(ser, idx, r)) { shortSum += r; shortCount++; }
        }

        double longRet = (longCount > 0) ? (longSum / longCount) : 0.0;
        double shortRet = (shortCount > 0) ? (shortSum / shortCount) : 0.0;
        double ls = longRet - shortRet;

        DailyResult dr;
        dr.date = d;
        dr.longReturn = longRet;
        dr.shortReturn = shortRet;
        dr.longShort = ls;
        dr.cumReturn = (results.empty() ? (1.0 + ls) : results.back().cumReturn * (1.0 + ls));
        results.push_back(dr);
    }

    return results;
}

double computeAnnualizedSharpe(const std::vector<DailyResult>& res, int tradingDays) {
    if (res.size() < 2) return 0.0;
    std::vector<double> rets; rets.reserve(res.size());
    for (const auto& r : res) rets.push_back(r.longShort);

    double mean = std::accumulate(rets.begin(), rets.end(), 0.0) / rets.size();
    double var = 0.0;
    for (double x : rets) var += (x - mean) * (x - mean);
    var /= rets.size();
    double vol = std::sqrt(var);

    if (vol == 0.0) return 0.0;
    double dailySharpe = mean / vol;
    return dailySharpe * std::sqrt((double)tradingDays);
}

double computeMaxDrawdown(const std::vector<DailyResult>& res) {
    if (res.empty()) return 0.0;
    double peak = res.front().cumReturn;
    double maxDD = 0.0;
    for (const auto& r : res) {
        peak = std::max(peak, r.cumReturn);
        double dd = (peak - r.cumReturn) / peak;
        if (dd > maxDD) maxDD = dd;
    }
    return maxDD;
}

bool writeResultsCSV(const std::string& path, const std::vector<DailyResult>& res) {
    std::ofstream f(path);
    if (!f.is_open()) return false;
    f << "date,long_return,short_return,long_short,cum_return\n";
    for (const auto& r : res) {
        f << r.date << "," << r.longReturn << "," << r.shortReturn << "," << r.longShort << "," << r.cumReturn << "\n";
    }
    f.close();
    return true;
}
