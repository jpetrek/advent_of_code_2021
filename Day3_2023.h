#pragma once
#include "helper.h"

template<>
class day<3, 2023> : public day_base<3,2023>
{
    void run_internal() override
    {
        auto data = utility::io::transform_input_into_array<char>(input_reader(), [](char c) { return c; });

        std::map<std::pair<size_t, size_t>, std::vector<size_t>> gears;

        size_t sum_star1 = 0;

        for (size_t y = 0; y < data.size(); y++)
        {
            for (size_t x = 0; x < data[0].size(); x++)
            {
                size_t number = 0;
                bool is_part_number = false;
                std::set<std::pair<size_t, size_t>> current_gears;
                while (x < data[0].size() && isdigit(data[y][x]))
                {
                    number *= 10;
                    number += data[y][x] - '0';
                    utility::geometry::do_for_adjacent(data[0].size(), data.size(), x, y, [&](size_t a, size_t b)
                        {
                            if ((!isdigit(data[b][a])) && (data[b][a] != '.'))
                            {
                                if (data[b][a] == '*')
                                {
                                    current_gears.insert({ a,b });
                                }
                                is_part_number = true;
                            }
                        });
                    x++;
                }
                if (is_part_number)
                {
                    sum_star1 += number;
                    for (const auto& g : current_gears)
                    {
                        gears[g].push_back(number);
                    }
                }
            }
        }

        size_t sum_star2 = 0;
        for (const auto& g : gears)
        {
            if (g.second.size() == 2)
            {
                sum_star2 += g.second[0] * g.second[1];
            }
        }

        set_star1_result(sum_star1);
        set_star2_result(sum_star2);
    }
};