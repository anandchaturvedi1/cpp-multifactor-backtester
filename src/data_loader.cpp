#include "../include/data_loader.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::vector<StockData> loadCSV(const std::string& filename) {
    std::vector<StockData> dataset;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << "\n";
        return dataset;
    }

    std::string line;
    bool header = true;

    while (std::getline(file, line)) {
        if (header) { // skip header row
            header = false;
            continue;
        }
        std::stringstream ss(line);
        StockData data;
        std::string open, high, low, close, volume;

        std::getline(ss, data.date, ',');
        std::getline(ss, data.ticker, ',');
        std::getline(ss, open, ',');
        std::getline(ss, high, ',');
        std::getline(ss, low, ',');
        std::getline(ss, close, ',');
        std::getline(ss, volume, ',');

        data.open = std::stod(open);
        data.high = std::stod(high);
        data.low = std::stod(low);
        data.close = std::stod(close);
        data.volume = std::stol(volume);

        dataset.push_back(data);
    }

    file.close();
    return dataset;
}
