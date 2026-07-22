#pragma once
#include <vector>
#include "DataLoader.h"

//represents a buy or sell signal
struct Signal {
    int barIndex;  //INDEX of bar signal happened on
    bool buy;      //true = buy, false = sell
    double price;  //price when signal fired
};

//takes all bars + fast and slow period to return a list of buy/sell signals
std::vector<Signal> movingAverageCrossover(const std::vector<Bar>& bars, int fastPeriod, int slowPeriod);

//calculates the average closing price of the last period (X amount) of bars ending at the index
double movingAverage(const std::vector<Bar>& bars, int barIndex, int period);
