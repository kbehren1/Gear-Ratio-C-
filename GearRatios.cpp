// GearRatios.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

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

int lookForNums(size_t& found, std::string Line1, std::string Line2, std::string Line3) {    
    size_t findPeriod = 0;
    bool validNum = false;
    std::vector<int> nums;

    std::string buildingNum = "";

    // Search Line1
    size_t searchPos = std::max(0, static_cast<int>(found - 3));
    while (searchPos < found + 3) {
        searchPos = Line1.find_first_of(numSearch, searchPos);
        findPeriod = Line1.find_first_of('.', searchPos);
        if (searchPos != std::string::npos) {
            while (searchPos < findPeriod) {
                buildingNum += Line1[searchPos];
                
                if (searchPos == found - 1 || searchPos == found || searchPos == found + 1)
                    validNum = true;

                searchPos = Line1.find_first_of(numSearch, searchPos + 1);
            }
        }

        if (validNum)
            nums.insert(nums.begin(), std::stoi(buildingNum));

        buildingNum = "";
        validNum = false;
    }

    // Search Line2 left side
    searchPos = std::max(0, static_cast<int>(found - 3));
    while (searchPos < found) {        
        searchPos = Line2.find_first_of(numSearch, searchPos);
        findPeriod = Line2.find_first_of('.', searchPos);
        
        if (searchPos != std::string::npos) {
            while (searchPos < findPeriod && searchPos < found) {
                buildingNum += Line2[searchPos];
                
                if (searchPos == found - 1)
                    validNum = true;

                searchPos = Line2.find_first_of(numSearch, searchPos + 1);
            }
        }
        
        if (validNum)
            nums.insert(nums.begin(), std::stoi(buildingNum));

        buildingNum = "";
        validNum = false;
    }

    // Search Line2 right side
    searchPos = std::max(0, static_cast<int>(found + 1));
    while (searchPos < found + 3) {
        searchPos = Line2.find_first_of(numSearch, searchPos);
        findPeriod = Line2.find_first_of('.', searchPos);

        if (searchPos != std::string::npos) {
            while (searchPos < findPeriod) {
                buildingNum += Line2[searchPos];
                
                if (searchPos == found + 1)
                    validNum = true;

                searchPos = Line2.find_first_of(numSearch, searchPos + 1);
            }
        }

        if (validNum)
            nums.insert(nums.begin(), std::stoi(buildingNum));

        buildingNum = "";
        validNum = false;
    }

    // Search Line3
    searchPos = std::max(0, static_cast<int>(found - 3));
    while (searchPos < found + 3) {
        searchPos = Line3.find_first_of(numSearch, searchPos);
        findPeriod = Line3.find_first_of('.', searchPos);
        if (searchPos != std::string::npos) {
            while (searchPos < findPeriod) {
                buildingNum += Line3[searchPos];
                
                if (searchPos == found - 1 || searchPos == found || searchPos == found + 1)
                    validNum = true;

                searchPos = Line3.find_first_of(numSearch, searchPos + 1);
            }
        }

        if (validNum)
            nums.insert(nums.begin(), std::stoi(buildingNum));

        buildingNum = "";
        validNum = false;
    }

    if (nums.size() == 2)
        return nums.at(0) * nums.at(1);
    else
        return 0;
}

int searchForStars(std::string Line1, std::string Line2, std::string Line3) {
    size_t found = 0;
    unsigned int gearRatio = 0;

    found = Line2.find_first_of('*');

    while (found != std::string::npos) {
        gearRatio += lookForNums(found, Line1, Line2, Line3);
        found = Line2.find_first_of('*', found + 1);
    }

    return gearRatio;
}

void handleLastLine(std::string& Line1, std::string& Line2, std::string& Line3, int& partNumSum, unsigned int& gearRatioSum) {
    setLines(Line1, Line2, Line3, "");
    partNumSum += searchForNum(Line1, Line2, Line3);
    gearRatioSum += searchForStars(Line1, Line2, Line3);
}

void processSchematic(int &partNumSum, unsigned int &gearRatioSum) {
    std::string Line1(140, ' ');
    std::string Line2(140, ' ');
    std::string Line3(140, ' ');
    std::string input = "";

    std::ifstream Schematic("Schematic.txt");

    while (std::getline(Schematic, input)) {
        setLines(Line1, Line2, Line3, input);
        partNumSum += searchForNum(Line1, Line2, Line3);
        gearRatioSum += searchForStars(Line1, Line2, Line3);
    }

    handleLastLine(Line1, Line2, Line3, partNumSum, gearRatioSum);
}

int main()
{   
    int partNumSum = 0;
    unsigned int gearRatioSum = 0;

    processSchematic(partNumSum, gearRatioSum);

    std::cout << "The sum of the part numbers in the engine schematic is " << partNumSum << "!\n";
    std::cout << "The sum of the gear ratios in the engine schematic is " << gearRatioSum << '!';
}


