#pragma once
#include <vector>
#include <string>
#include "DataLoader.h"
#include "Strategy.h"

//represents one completed trade
struct Trade {
    std::string buyDate;
    std::string sellDate;
    double buyPrice;
    double sellPrice;
    double profitLoss;
};

//tracks cash,shares & trades over time
struct Portfolio {
    double cash; //current cash
    double shares; //current shares owned
    std::vector<Trade> trades; //list of completed trades
    std::vector<std::pair<std::string, double>> valueOverTime; //date + total value in portfolio
};

Portfolio runBackTest(const std::vector<Bar>& bars, const std::vector<Signal>& signals, double startingMoney);

