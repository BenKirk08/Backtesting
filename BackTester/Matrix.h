#pragma once
#include <array>
#include <cmath>

//Makes a shorthand definition for readabililty
using Vec2 = std::array<double, 2>;
using Mat2 = std::array<std::array<double, 2>, 2>;
//inline for short pure functions

//multiply
inline Mat2 multiply(const Mat2& MatA, const Mat2& MatB) {
    Mat2 MatC{}; //initilise -> as 0
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                MatC[i][j] += MatA[i][k] * MatB[k][j];
            }
        }
    }
    return MatC;
}

//Transpose
inline Mat2 transpose(const Mat2& MatA) {
    return {{{MatA[0][0], MatA[1][0]},
             {MatA[0][1], MatA[1][1]}}};
}

//Determinant
inline double determinant(const Mat2& MatA) {
    return MatA[0][0] * MatA[1][1] - MatA[0][1] * MatA[1][0];
}

//Inverse (2x2)
inline Mat2 inverse(const Mat2& MatA) {
    double detA = determinant(MatA);
    if (std::abs(detA) < 1e-12) throw std::runtime_error("tried inverse singular matrix");

    Mat2 inverseMat;
    inverseMat[0][0] = MatA[1][1] / detA;
    inverseMat[0][1] = -MatA[0][1] / detA;
    inverseMat[1][0] = -MatA[1][0] / detA;
    inverseMat[1][1] = MatA[0][0] / detA;
    return inverseMat;
}

//eigenvalues of a 2x2 ->solves ev^2 - trace.ev + det = 0 (quad formula)
//returns {larger, smaller}
inline Vec2 eigenValues(const Mat2& MatA) {
    double detMatA = determinant(MatA);
    double trace = MatA[0][0] + MatA[1][1];
    double discriminateMatA = trace*trace - 4.0*detMatA; //b^2 - 4ac
    if (discriminateMatA < 0) discriminateMatA = 0; //clamp small negative values (for sqrt)

    double root = std::sqrt(discriminateMatA);
    return {(trace + root) / 2.0, (trace - root) / 2.0};
}