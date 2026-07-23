#pragma once
#include "Kalman.h"
#include <cstddef>
#include <vector>

//z-score of each spread vs the trailing window the days before it (excludes today)
//first window entires are N/A as not enough data therfore no signal
std::vector<double> rollingZScore(const std::vector<double>& spread, size_t window);
void outputZScoreResults(std::vector<double> z, KalmanOutput ko);