// AOC2021.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define PATH_TO_DATA(YEAR,DAY) "c:\\Git\\aoc\\" + std::to_string(YEAR) + "\\i" + std::to_string(DAY) + ".txt"

#include "days.h"

int main()
{
    helper::run_all<2022, 5>(); 
    //run_day<day<5,2022>>();
    return 0;
}
