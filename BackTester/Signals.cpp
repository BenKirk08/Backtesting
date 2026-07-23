#include "Signals.h"
#include "Kalman.h"
#include <cmath>
#include <cstddef>
#include <limits>
#include <vector>
#include <iostream>

std::vector<double> rollingZScore(const std::vector<double>& spread, size_t window) {
    //list of size 60 and prefill everything with NaN
    std::vector<double> z(spread.size(), std::numeric_limits<double>::quiet_NaN());

    for (size_t t = window; t < spread.size(); t++) {
        //mean of the window ending yesterday [t-window, t-1]
        double mean = 0;
        for (size_t i = (t-window); i < t; i++) {
            mean += spread[i];
        }
        mean /= window;

        //standard deviation of that window
        double var = 0;
        for (size_t i = (t-window); i < t; i++) { //variance = (x_i-mean)^2/n
            var += ((spread[i]-mean) * (spread[i] - mean));
        }
        var /= window;
        double sd = std::sqrt(var);

        z[t] = (spread[t] - mean) / sd; //how many sds from normal is today
    }
    return z;;
}

void outputZScoreResults(std::vector<double> z, KalmanOutput ko) {
    size_t isBig = 0, valid = 0;
    double zmax = 0; std::string zmaxDate;
    for (size_t i = 0; i < z.size(); i++) {
        if (std::isnan(z[i])) continue;
        valid++;
        if (std::abs(z[i]) > 2.0) isBig++;
        if (std::abs(z[i]) > std::abs(zmax)) { 
            zmax = z[i]; 
            zmaxDate = ko.dates[i]; 
        }
    }
    std::cout << "days with |z|>2: " << isBig << " / " << valid
              << " (" << (100.0 * isBig / valid) << "%)\n";
    std::cout << "most extreme z = " << zmax << " on " << zmaxDate << "\n";
}