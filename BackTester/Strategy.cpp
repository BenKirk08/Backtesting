#include "Strategy.h"

double movingAverage(const std::vector<Bar> &bars, int barIndex, int period) {
    double sum = 0;
    for (int i = barIndex - period + 1; i <= barIndex; i++) {
        sum += bars[i].close;
    }
    return sum / period;
}

std::vector<Signal> movingAverageCrossover(const std::vector<Bar>& bars, int fastPeriod, int slowPeriod) {
    std::vector<Signal> signals; //list of signals to return

    //start at slowPeriod so have enough bars to calculate slow MAs
    for (int i = slowPeriod; i < bars.size(); i++) {
        double fastNow = movingAverage(bars, i, fastPeriod);        //fast MA today
        double slowNow = movingAverage(bars, i, slowPeriod);        //slow MA today
        double fastPrev = movingAverage(bars, i - 1, fastPeriod);   //fast MA yesterday
        double slowPrev = movingAverage(bars, i - 1, slowPeriod);   //slow MA yesterday

        //fast crossed above slow -> buy signal
        if (fastPrev < slowPrev && fastNow > slowNow) {
            signals.push_back({i, true, bars[i].close});
        }

        //fast crossed below slow -> sell signal
        if (fastPrev > slowPrev && fastNow < slowNow) {
            signals.push_back({i, false, bars[i].close});
        }
    }
    return signals;
}
