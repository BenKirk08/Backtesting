#include "DataLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <cmath>

std::vector<Bar> loadCSV(const std::string& filename) {
    std::vector<Bar> bars;
    std::ifstream file(filename);

    if (!file.is_open()) { //error if cant open file
        std::cout << "Error: couldnt open file: " << filename;
    }
    std::string line;

    std::getline(file, line); //skip header

    while (std::getline(file, line)) {
        if (line.empty()) continue; //skip empty lines
        
        std::stringstream ss(line);
        std::string tempString;
        Bar thisBar;

        std::getline(ss, thisBar.date, ',');
        std::getline(ss, tempString, ','); thisBar.open = std::stod(tempString);
        std::getline(ss, tempString, ','); thisBar.high = std::stod(tempString);
        std::getline(ss, tempString, ','); thisBar.low = std::stod(tempString);
        std::getline(ss, tempString, ','); thisBar.close = std::stod(tempString);
        std::getline(ss, tempString, ','); thisBar.volume = std::stoll(tempString);
        
        bars.push_back(thisBar);
    }
    return bars;
}


//two pointer merge -> keep bars present in both series and drop unmatched days
std::vector<AlignedBar> alignBars(const std::vector<Bar>& barsA, const std::vector<Bar>& barsB) {
    std::vector<AlignedBar> aligned;
    size_t i = 0, j = 0; //useful in .size() comparisons (same data type) -> avoids error

    while (i < barsA.size() && j < barsB.size()) {
        if (barsA[i].date == barsB[j].date) { //can compare string as same format
            aligned.push_back({barsA[i].date, barsA[i].close, barsB[j].close, std::log(barsA[i].close), std::log(barsB[j].close)});
            i++; //log -> natural log (NOT base 10 but base e)
            j++;
        } else if (barsA[i].date < barsB[j].date) {
            i++; //A has an earlier date B doesn't have, skip it
        } else {
            j++; //B has an earlier date A doesn't have, skip it
        }
    }
    return aligned;
}