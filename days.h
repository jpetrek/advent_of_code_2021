#pragma once
#include "Day1.h"
#include "Day2.h"
#include "Day3.h"
#include "Day4.h"
#include "Day5.h"
#include "Day6.h"
#include "Day7.h"
#include "Day8.h"
#include "Day9.h"
#include "Day10.h"
#include "Day11.h"
#include "Day12.h"
#include "Day13.h"
#include "Day14.h"
#include "Day15.h"
#include "Day16.h"
#include "Day17.h"
#include "Day18.h"
#include "Day19.h"
#include "Day20.h"
#include "Day21.h"
#include "Day22.h"
#include "Day23.h"
#include "Day24.h"

template <class T>
static void run_day()
{
    T day;
    day.run();
}

void run_all()
{
    run_day<day1>();
    run_day<day2>();
    run_day<day3>();
    run_day<day4>();
    run_day<day5>();
    run_day<day6>();
    run_day<day7>();
    run_day<day8>();
    run_day<day9>();
    run_day<day10>();
    run_day<day11>();
    run_day<day12>();
    run_day<day13>();
    run_day<day14>();
    run_day<day15>();
    run_day<day16>();
    run_day<day17>();
    run_day<day18>();
    run_day<day19>();
    run_day<day20>();
    run_day<day21>();
    run_day<day22>();
    run_day<day23>();
    run_day<day24>();
}