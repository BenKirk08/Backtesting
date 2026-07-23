#include "Kalman.h"
#include "Matrix.h"
#include <iostream>

//step 1: Initilise :)
KalmanFilter initiliseKalman(const AlignedBar &firstBar, double Q, double R) {
    KalmanFilter kf;
    //beta = 1, alpha is diffence (to start)
    kf.x = {firstBar.lnCloseA - firstBar.lnCloseB, 1.0}; 
    kf.P = {{{0.01, 0},
            {0, 0.01}}}; //start P medium -> mostly unsure about both
    kf.Q = Q;
    kf.R = R;
    return kf;
}

double kalmanStep(KalmanFilter& kf, const AlignedBar& bar) {
    double b = bar.lnCloseB;
    double y = bar.lnCloseA;

    //step 2: predict -> state unchanged but increase uncertanty :)
    kf.P[0][0] += kf.Q;
    kf.P[1][1] += kf.Q;

    //step 3&4: What A should be vs what it is :) NB// lnA PREDICTION = alpha + beta.lnb
    double e = y - (kf.x[0] + kf.x[1] * b); //e = lnA - (alpha + beta.lnb)

    //step 5: kalman gain. S = H.P.H' + R, K = P.H'/S :)
    double S = kf.P[0][0] + b * (kf.P[0][1] + kf.P[1][0]) + b*b*kf.P[1][1] + kf.R;
    Vec2 K = {(kf.P[0][0] + b*kf.P[0][1]) / S,
              (kf.P[1][0] + b*kf.P[1][1]) / S};

    //step 6: update - > correct state and shrink uncertainty P = (I - K.H).P, x = x + K.e :)
    kf.x[0] += K[0] * e;
    kf.x[1] += K[1] * e;
    Mat2 I_KH = {{{1.0 - K[0], -K[0] * b},
                -K[1], 1.0 - K[1] * b}};
    kf.P = multiply(I_KH, kf.P);
    return e; //update P and return spread
}

KalmanOutput runKalman(const std::vector<AlignedBar>& bars, double Q, double R) {
    KalmanOutput kOut;
    KalmanFilter kf = initiliseKalman(bars[0], Q, R);

    for (size_t t = 1; t < bars.size(); t++) {
        double e = kalmanStep(kf, bars[t]);
        kOut.dates.push_back(bars[t].date);
        kOut.beta.push_back(kf.x[1]);
        kOut.spread.push_back(e);
    }
    return kOut;
}

void outputKalmanResults(KalmanOutput ko) {
    for (size_t i = 0; i < ko.beta.size(); i += 250)
        std::cout << ko.dates[i] << "  beta = " << ko.beta[i] << "  spread = " << ko.spread[i] << "\n";
    double bmin = 1e9, bmax = -1e9;
    for (double b : ko.beta) { 
        if (b < bmin) bmin = b; 
        if (b > bmax) bmax = b; 
    }
    std::cout << "beta range: [" << bmin << ", " << bmax << "]\n\n";
}