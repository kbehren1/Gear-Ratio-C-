// GearRatios.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>

int main()
{
    const std::string numSearch = "0123456789";
    const std::string symbolSearch = "*$&%@+/#=-";
    std::string Line1 = "";
    std::string Line2 = "";
    std::string Line3 = "";
    std::string input = "";      
    int partNumSum = 0;
    int partNum = 0;
    size_t found = 0;

    std::ifstream Schematic("Schematic.txt");

    while (std::getline(Schematic, input)) {
        Line1 = Line2;
        Line2 = Line3;
        Line3 = input;

        found = 0;
        partNum = 0;        

        if (Line2 == "")
            continue;
        else {
            found = Line2.find_first_of(numSearch);

            while (found != std::string::npos) {
                int searchLength = 0;
                std::string buildingNum = "";
                // If we're starting our compares at the beginning of the line
                if (found == 0) {
                    while (found != std::string::npos && Line2.find('.') > found) {
                        buildingNum += Line2[found];
                        found = Line2.find_first_of(numSearch, found + 1);
                    }
                    searchLength = buildingNum.length() + 1;
                    if (Line1.substr(0, searchLength).find_first_of(symbolSearch) != std::string::npos || Line3.substr(0, searchLength).find_first_of(symbolSearch) != std::string::npos) {
                        partNum = std::stoi(buildingNum);
                        partNumSum += partNum;
                    }
                }
                else {
                    size_t beginPos = found;
                    
                    buildingNum = Line2.substr(beginPos, Line2.find_first_of("." + symbolSearch, beginPos)-beginPos);

                    searchLength = buildingNum.length() + 2;
                    
                    size_t subBeginPos = std::max(0, static_cast<int>(beginPos - 1));
                    
                    if (Line1.substr(std::min(subBeginPos, Line1.length()), searchLength).find_first_of(symbolSearch) != std::string::npos || 
                        Line3.substr(subBeginPos, searchLength).find_first_of(symbolSearch) != std::string::npos ||
                        Line2.substr(subBeginPos, 1).find_first_of(symbolSearch) != std::string::npos ||
                        Line2.substr(subBeginPos + searchLength - 1, 1).find_first_of(symbolSearch) != std::string::npos) {

                        partNum = std::stoi(buildingNum);
                        partNumSum += partNum;
                    }
                    found = Line2.find_first_of(numSearch, beginPos + searchLength - 1);
                }                
            }
        }
    }

    //Read last line of file
    found = Line3.find_first_of(numSearch);

    while (found != std::string::npos) {
        int searchLength = 0;
        std::string buildingNum = "";
        // If we're starting our compares at the beginning of the line
        if (found == 0) {
            while (found != std::string::npos && Line3.find('.') > found) {
                buildingNum += Line3[found];
                found = Line3.find_first_of(numSearch, found + 1);
            }
            searchLength = buildingNum.length() + 1;
            if (Line2.substr(0, searchLength).find_first_of(symbolSearch) != std::string::npos) {
                partNum = std::stoi(buildingNum);
                partNumSum += partNum;
            }
        }
        else {
            size_t beginPos = found;
            
            buildingNum = Line3.substr(beginPos, Line3.find_first_of("." + symbolSearch, beginPos) - beginPos);

            searchLength = buildingNum.length() + 2;

            size_t subBeginPos = std::max(0, static_cast<int>(beginPos - 1));
            
            if (Line2.substr(subBeginPos, searchLength).find_first_of(symbolSearch) != std::string::npos ||
                Line3.substr(subBeginPos, 1).find_first_of(symbolSearch) != std::string::npos ||
                Line3.substr(subBeginPos + searchLength - 1, 1).find_first_of(symbolSearch) != std::string::npos) {

                partNum = std::stoi(buildingNum);
                partNumSum += partNum;
            }
            found = Line3.find_first_of(numSearch, beginPos + searchLength - 1);
        }
    }

    std::cout << "The sum of the part numbers in the engine schematic is " << partNumSum << '!';
}


