#include "Cointegration.h"
#include "Matrix.h"
#include <cstddef>

Residuals buildResiduals(const std::vector<AlignedBar>& bars) {
    size_t NoBars = bars.size();
    Residuals residuals;

    //build raw Δy_t and lagged level y_t-1 -> starting at t = 1
    std::vector<Vec2> dy; //Δy_t
    std::vector<Vec2> yLag; //y_t-1
    for (size_t t = 1; t < NoBars; t++) {
        double dA = bars[t].lnCloseA - bars[t-1].lnCloseA; //change in ln(Stock1)
        double dB = bars[t].lnCloseB - bars[t-1].lnCloseB; //change in ln(Stock2)
        dy.push_back({dA, dB}); //add both change in stocks
        yLag.push_back({bars[t-1].lnCloseA, bars[t-1].lnCloseB});
    }

    size_t NoUseableRows = dy.size(); //(NoBars-1)

    //column menas of each series
    double meanDyA = 0, meanDyB = 0, meanYA = 0, meanYB = 0;
    for (size_t i = 0; i < NoUseableRows; i++) {
        meanDyA += dy[i][0]; meanDyB += dy[i][1];
        meanYA += yLag[i][0]; meanYB += yLag[i][1];
    }
    meanDyA /= NoUseableRows; meanDyB /= NoUseableRows;
    meanYA /= NoUseableRows; meanYB /= NoUseableRows;

    //subtract means from every value -> get residuals R0 & R1
    for (size_t i = 0; i < NoUseableRows; i++) {
        residuals.R0.push_back({dy[i][0] - meanDyA, dy[i][1] - meanDyB});
        residuals.R1.push_back({yLag[i][0] - meanYA, yLag[i][1] - meanYB});
    }

    return residuals; //what short term runs cant explain (->cleaned)
}

MomentMatrices computeMoments(const Residuals & residuals) {
    size_t NoRows = residuals.R0.size();
    MomentMatrices S{}; //initilise all 4

    //sum of outer products over every row
    for (size_t k = 0; k < NoRows; k++ ){
        for (size_t i = 0; i < 2; i++) {
            for (size_t j = 0; j < 2; j++) {
                S.S00[i][j] += residuals.R0[k][i] * residuals.R0[k][j];
                S.S11[i][j] += residuals.R1[k][i] * residuals.R1[k][j];
                S.S01[i][j] += residuals.R0[k][i] * residuals.R1[k][j];
            }
        }
    }

    //divide number by roes -> sums become averages
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            S.S00[i][j] /= NoRows;
            S.S11[i][j] /= NoRows;
            S.S01[i][j] /= NoRows;
        }
    } //Since residuals Mean is 0 -> calcuating variances E(X)^2 - 0
    S.S10 = transpose(S.S01); //S01 is same but transposed -> save time computing

    return S; //moment matrices: S00/S11 -> movement fluctuates/levels varies 
              //S01/S10 = the movement & level link the test measures
}

//original eqn: det(eigenV.S11 − S10.S00^-1.S01) = 0
//->det(S11) . det(eigenV.I − S11^-1.S10.S00^-1.S01) = 0
//->det(S11) not 0 so divide out
//->det(eigenV.I - K) = 0 where K = S11^-1.S10.S00^-1.S01

//K = S11^-1.S10.S00^-1.S01 -> eigenvalues measure strength of the level to chnage link
Vec2 johansenEigenvalues(const MomentMatrices& S) {
    Mat2 K = multiply(multiply(multiply(
    inverse(S.S11), S.S10), inverse(S.S00)), S.S01);
    return eigenValues(K);
}

//at 5% the Critical Values for 2 series, constant, no trend
const double CV_RANK0 = 15.4943;
const double CV_RANK1 = 3.8415;

//run full test -> used in main
JohansenResult johansenTest(const std::vector<AlignedBar>& bars) {
    //get residuals from bars and build moment matrices from them to get eigen Values
    Residuals residuals = buildResiduals(bars);
    MomentMatrices S = computeMoments(residuals);
    Vec2 eigenValues = johansenEigenvalues(S);

    double T = (double)residuals.R0.size(); //no of useable rows

    JohansenResult result;
    //Trace0 = -T[ln(1-ev1) + ln(1-ev2)]
    result.trace0 = -T * (std::log(1.0 - eigenValues[0]) + std::log(1.0 - eigenValues[1]));
    result.trace1 = -T * std::log(1.0 - eigenValues[1]); //second relation (presumes larger value already)

    //cointegreation -> reject trace0 and accept trace1 therefore exactly 1 
    result.isCointegrated = (result.trace0 > CV_RANK0) && (result.trace1 < CV_RANK1);
    return result;
}

//SUMMARY
//Johansen test w/constant term no trend for cointegration (series intgerated of order 1 -> stationary after first differnece)
//answers a yes/no question: does an anchored relationship exist using a single averaged hedge ratio 