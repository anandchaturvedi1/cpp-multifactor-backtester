#ifndef DATA_LOADER_H
#define DATA_LOADER_H

#include <string>
#include <vector>

struct StockData {
    std::string date;
    std::string ticker;
    double open;
    double high;
    double low;
    double close;
    long volume;
};

std::vector<StockData> loadCSV(const std::string& filename);

#endif
