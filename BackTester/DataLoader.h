#pragma once
#include <string>
#include <vector>

struct Bar {
    std::string date;
    double open, high, low, close;
    long long volume;
};
std::vector<Bar> loadCSV(const std::string& filename);

struct AlignedBar {
    std::string date;
    double closeA, closeB;
    double lnCloseA, lnCloseB;
};
std::vector<AlignedBar> alignBars(const std::vector<Bar>& barsA, const std::vector<Bar>& barsB);