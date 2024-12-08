#pragma once
#include "helper.h"
#include <random>
#include <cmath>

using namespace utility::arrays;
using namespace utility::geometry;
using namespace utility::math;
using namespace utility::string;

template<>
class day<24, 2021> : public day_base<24, 2021>
{
    void run_internal() override
    {
        std::vector<std::vector<std::string>> lines;
        std::stack<std::pair<int64_t, int64_t>> stack;
        std::vector<size_t> part_1(14, 0);
        std::vector<size_t> part_2(14, 0);

        while (!input_reader().is_end_of_file())
        {
            auto s = split(input_reader().get_line(), ' ');
            lines.push_back(s);
        }

        for (size_t i = 0; i < 14; i++)
        {
            if (lines[18 * i + 4][2] == "1")
            {
                stack.push({ i, std::stoi(lines[18 * i + 15][2])});
            }
            else
            {
                auto item = stack.top(); stack.pop();
                int64_t inc = item.second + std::stoi(lines[18 * i + 5][2]);
                if (inc > 0)
                {
                    part_1[i] = 9;
                    part_1[item.first] = 9 - inc;
                    part_2[i] = 1 + inc;
                    part_2[item.first] = 1;
                }
                else
                {
                    part_1[i] = 9 + inc;
                    part_1[item.first] = 9;
                    part_2[i] = 1;
                    part_2[item.first] = 1 - inc;
                }
            }
        }
    }
};
