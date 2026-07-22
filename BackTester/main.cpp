#include "DataLoader.h"
#include "Cointegration.h"
#include "Portfolio.h"
#include "Strategy.h"
#include "Menu.h"
#include <string>
#include <vector>
#include <iostream>

//3 different Relationships 1. cointegrated, 2. borderline cointegrated (reject), 3. Not cointegreated
const std::string CUK_1 = "Data/Input/cuk_Data.csv";
const std::string CCL_1 = "Data/Input/ccl_Data.csv";
const std::string XOM_2 = "Data/Input/xom_Data.csv";
const std::string CVX_2 = "Data/Input/cvx_Data.csv";
const std::string COLA_3 = "Data/Input/cola_Data.csv";
const std::string PEPSI_3 = "Data/Input/pepsi_Data.csv";

int main() {
    //load all bars from csv file
    std::vector<Bar> barsA = loadCSV(CUK_1);
    std::vector<Bar> barsB = loadCSV(CCL_1);
    std::vector<AlignedBar> alignedBars = alignBars(barsA, barsB);
    
    JohansenResult CoInResult = johansenTest(alignedBars);
    std::cout << (CoInResult.isCointegrated? "IS cointegrated at 5%\n" : "is NOT cointegrated at 5%\n");

    // //generate signals with 10 day fast and 50 day slow MA
    // std::vector<Signal> signals = movingAverageCrossover(bars, 10, 50);

    // //run the backtest using bars and signals then print results
    // const double startingMoney = 10000;
    // Portfolio resultPortfolio = runBackTest(bars, signals, startingMoney);

    // //Now has been ran, choose what to do
    // int option = -1;
    // do {
    //     option = DisplayMenuGetChoice();
    //     switch (option) {
    //         case 1:
    //             exportJSON(resultPortfolio, bars, startingMoney);
    //             OpenChartInBrowser();
    //             break;
    //         case 2:
    //             printResults(resultPortfolio, bars);
    //             break;
    //     }
    // } while (option != 3);

    return 0;
}




