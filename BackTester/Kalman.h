#pragma once
#include <vector>
#include "DataLoader.h"
#include "Matrix.h"

struct KalmanFilter {
    Vec2 x; //state [alpha, beta]
    Mat2 P; //current uncertainty about the state
    double Q; //how much true state can drift per day
    double R; //how noisy one days price is
};

KalmanFilter initiliseKalman(const AlignedBar& firstBar, double Q, double R);

//predict + update for one day. returns that days spread
double kalmanStep(KalmanFilter& kalmanFilter, const AlignedBar& bar);

struct KalmanOutput {
    std::vector<std::string> dates;
    std::vector<double> beta; //hedge ratio series
    std::vector<double> spread; //surprise/e/mispricing series
};

KalmanOutput runKalman(const std::vector<AlignedBar>& bars, double Q, double R);
void outputKalmanResults(KalmanOutput ko);

