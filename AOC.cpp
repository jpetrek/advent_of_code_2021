// AOC2021.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#define PATH_TO_DATA(YEAR,DAY) "c:\\Git\\aoc\\" + std::to_string(YEAR) + "\\i" + std::to_string(DAY) + ".txt"
#define PATH_TO_TEST_DATA(YEAR,DAY) "c:\\Git\\aoc\\" + std::to_string(YEAR) + "\\i" + std::to_string(DAY) + "_t.txt"

#include "days.h"

int main()
{
    helper::run<2023,24, 24 >(); 
    return 0;
}
