#pragma once
#include <vector>
#include "DataLoader.h"
#include "Matrix.h" //bring Vec2 & Mat2

//holds the two demeaned redisual series -> i.e. what short run terms cant explain 
struct Residuals {
    std::vector<Vec2> R0; //cleaned changes (Δy_t)
    std::vector<Vec2> R1; //cleaned levels (y_t-1)
};

//the 4 moment matices
struct MomentMatrices{
    Mat2 S00, S11, S01, S10;
};

struct JohansenResult {
    double trace0; //test for H0 -> no cointegration
    double trace1; //test for H0 -> at most 1 relation
    bool isCointegrated; //true -> exactly one cointegrating relation at 5%
};

Residuals buildResiduals(const std::vector<AlignedBar>& bars);
MomentMatrices computeMoments(const Residuals& residuals);
Vec2 johansenEigenvalues(const MomentMatrices& S);
JohansenResult johansenTest(const std::vector<AlignedBar>& bars);