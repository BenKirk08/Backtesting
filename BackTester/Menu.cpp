#include <iostream>
#include <fstream>
#include <limits>
#include <cstdlib> //for system
#include "Portfolio.h"

int DisplayMenuGetChoice() {
    int option = -1;
    do {
        std::cout << "-------BackTesting Menu-------\n"
                << "1. Display Results in Browser\n"
                << "2. Print Results In Terminal\n"
                << "3. Exit Application\n"
                << "Option: ";
        if (!(std::cin >> option)) { //if output is invalid -> stream returns false
            std::cin.clear(); //clear stream so reusable
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //remove letters from buffer (up to & including newline)
            option = -1; //reset
            std::cout << "\n\n";
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //i.e. if 2ab reads 2 -> correct but ab stays, remove to clear rest of buffer
        std::cout << "\n";
    } while (!(option >= 1 && option <= 3));
    return option;
}

//JSON -> html
void exportJSON(const Portfolio &portfolio, const std::vector<Bar> &bars, double startingMoney) {
    std::ofstream file("Data/Output/results.json");
    file << "{\n";

    //portfolio vaule -> date, portfolio value each day
    file << "  \"startingMoney\": " << startingMoney << ",\n";
    file << "  \"portfolioValue\": [\n";
    for (int i = 0; i < (int)portfolio.valueOverTime.size(); i++) {
        file << "    {\"date\": \"" << portfolio.valueOverTime[i].first
             << "\", \"value\": " << portfolio.valueOverTime[i].second << "}";
        if (i < (int)portfolio.valueOverTime.size() - 1) file << ",";
        file << "\n";
    }
    file << " ],\n";

    //trades -> buy date, sell date, buy price, sell price, P&L
    file << "  \"trades\": [\n";
    for (int i = 0; i < (int)portfolio.trades.size(); i++) {
        file << "    {\"buyDate\": \"" << portfolio.trades[i].buyDate
             << "\", \"buyPrice\": " << portfolio.trades[i].buyPrice
             << ", \"sellDate\": \"" << portfolio.trades[i].sellDate
             << "\", \"sellPrice\": " << portfolio.trades[i].sellPrice
             << ", \"profitLoss\": " << portfolio.trades[i].profitLoss << "}";
        if (i < (int)portfolio.trades.size() - 1) file << ","; //adds comma after each set expect last
        file << "\n";
    }
    file << "  ]\n";

    file << "}\n";

}

void OpenChartInBrowser() {
    system("lsof -ti:8080 | xargs kill -9 2>/dev/null"); //kill any exisitng open in 8080
    system("sleep 1"); //wait for it to fully die
    system("cd ~/Desktop/BackTester/Data/Output && python3 -m http.server 8080 &"); //start new sever
    system("sleep 1");
    system("open http://localhost:8080/chart.html"); //open broswer
    system("sleep 2"); //keep alive for browser to connect
    std::cout << "\n";
}

//in terminal
void printResults(const Portfolio& portfolio, const std::vector<Bar>& bars) {
    std::cout << "\n===BACKTEST RESULTS===\n";
    std::cout << "Total trades: " << portfolio.trades.size() << "\n";
    for (const Trade& trade : portfolio.trades) {
        std::cout << trade.buyDate << " BUY at " << trade.buyPrice << " | " 
            << trade.sellDate << " SELL at " << trade.sellPrice << " | P&L: " << trade.profitLoss << "\n";
    }

    //if still holding shares at end
    if (portfolio.shares > 0) {
        double lastPrice = bars.back().close; //last closing price
        double estimatedValue = portfolio.shares * lastPrice;
        std::cout << "Still holding shares | Estimated Value: " << estimatedValue << "\n";
    }
    else {
        std::cout << "Final cash: " << portfolio.cash << "\n";
    }
    std::cout << "\n\n";
}





//make 2 csv files -> for python display
//1. trades.csv -> buy date, sell date, buy price, sell price, P&L
//2. portfolio_value.csv -> date, portfolio value each day
// void exportResults(const Portfolio& portfolio) {
//     //trades
//     std::ofstream tradesFile("Data/Output/trades.csv");
//     tradesFile << "BuyDate,BuyPrice,SellDate,SellPrice,ProfitLoss\n";
//     for (const Trade& trade : portfolio.trades) {
//         tradesFile << trade.buyDate << ","
//                    << trade.buyPrice << ","
//                    << trade.sellDate << ","
//                    << trade.sellPrice << ","
//                    << trade.profitLoss << "\n";
//     }
//     //portfolio value over time
//     std::ofstream valueFile("Data/Output/portfolio_value.csv");
//     valueFile << "Date,Value\n";
//     for (const std::pair<std::string, double>& values : portfolio.valueOverTime) {
//         valueFile << values.first << "," << values.second << "\n";
//     }
//}
