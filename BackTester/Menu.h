#pragma once 
#include "Portfolio.h"

int DisplayMenuGetChoice();

void exportJSON(const Portfolio& portfolio, const std::vector<Bar>& bars, double startingMoney);
void OpenChartInBrowser();
void printResults(const Portfolio& portfolio, const std::vector<Bar>& bars);

//void exportResults(const Portfolio& portfolio);