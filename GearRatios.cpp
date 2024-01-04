// GearRatios.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>

const std::string numSearch = "0123456789";

void setLines(std::string &Line1, std::string &Line2, std::string &Line3, std::string input) {
    Line1 = Line2;
    Line2 = Line3;
    if (input == "")
        std::fill(std::begin(Line3), std::end(Line3), ' ');
    else
        Line3 = input;
}

int lookForSymbols(size_t &found, std::string Line2, std::string Line1, std::string Line3) {
    const std::string symbolSearch = "*$&%@+/#=-";
    std::string buildingNum = "";
    int partNum = 0;

    buildingNum = Line2.substr(found, Line2.find_first_of("." + symbolSearch, found) - found);

    int searchLength = (found == 0) ? buildingNum.length() + 1 : buildingNum.length() + 2;

    size_t subBeginPos = std::max(0, static_cast<int>(found - 1));

    if (Line1.substr(std::min(subBeginPos, Line1.length()), searchLength).find_first_of(symbolSearch) != std::string::npos ||
        Line3.substr(subBeginPos, searchLength).find_first_of(symbolSearch) != std::string::npos ||
        Line2.substr(subBeginPos, 1).find_first_of(symbolSearch) != std::string::npos ||
        Line2.substr(subBeginPos + searchLength - 1, 1).find_first_of(symbolSearch) != std::string::npos) {

        partNum = std::stoi(buildingNum);
    }
    found = Line2.find_first_of(numSearch, found + searchLength - 1);

    return partNum;
}

int searchForNum(std::string Line1, std::string Line2, std::string Line3) {
    size_t found = 0;
    int partNums = 0;

    found = Line2.find_first_of(numSearch);

    while (found != std::string::npos)
        partNums += lookForSymbols(found, Line2, Line1, Line3);

    return partNums;
}

int handleLastLine(std::string &Line1, std::string &Line2, std::string &Line3) {
    setLines(Line1, Line2, Line3, "");
    return searchForNum(Line1, Line2, Line3);
}

int processSchematic() {
    std::string Line1(140, ' ');
    std::string Line2(140, ' ');
    std::string Line3(140, ' ');
    std::string input = "";
    int partNumSum = 0;

    std::ifstream Schematic("Schematic.txt");

    while (std::getline(Schematic, input)) {
        setLines(Line1, Line2, Line3, input);
        partNumSum += searchForNum(Line1, Line2, Line3);
    }

    return partNumSum + handleLastLine(Line1, Line2, Line3);
}

int main()
{   
    std::cout << "The sum of the part numbers in the engine schematic is " << processSchematic() << '!';
}


