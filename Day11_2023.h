#pragma once
#include "helper.h"

template<>
class day<11, 2023> : public day_base<11,2023>
{
    void run_internal() override
    {
        const size_t star1_expansion_coefficient = 2;
        const size_t star2_expansion_coefficient = 1000000;

        //set_is_test(true);
        std::set<std::pair<size_t, size_t>> galaxies;
        std::size_t w = 0;
        std::size_t h = 0;
        std::set<size_t> columns_with_galaxy;
        std::vector<size_t> expandable_rows;
        std::vector<size_t> expandable_columns;
        while (!input_reader().is_end_of_file())
        {
            auto l = input_reader().get_line();
            w = l.size();
            if (std::any_of(l.begin(), l.end(), [](const auto c) {return c == '#'; }))
            {
                for (size_t i = 0; i < w; i++)
                {
                    if (l[i] == '#')
                    {
                        galaxies.insert({ i,h });
                        columns_with_galaxy.insert(i);
                    }
                }

            }
            else
            {
                expandable_rows.push_back(h);
            }
            h++;
        }

        for (size_t x = 0; x < w; x++)
        {
            if (!columns_with_galaxy.contains(x)) expandable_columns.push_back(x);
        
        }

        size_t sum_s1 = 0;
        size_t sum_s2 = 0;
        
        for (const auto& g1 : galaxies)
        {
            for (const auto& g2 : galaxies)
            {
                if (g1 != g2)
                {
                    size_t number_of_column_there = 0;
                    size_t number_of_rows_there = 0;

                    for (const auto& r : expandable_columns)
                    {
                        if (r > std::min(g1.first, g2.first) && r < std::max(g1.first, g2.first))
                        {
                            number_of_column_there++;
                        }
                    }
                    for (const auto& r : expandable_rows)
                    {
                        if (r > std::min(g1.second, g2.second) && r < std::max(g1.second, g2.second))
                        {
                            number_of_rows_there++;
                        }
                    }

                    size_t dx = std::max(g1.first, g2.first) - std::min(g1.first, g2.first);
                    size_t dy = std::max(g1.second, g2.second) - std::min(g1.second, g2.second);
                    sum_s1 += dx + number_of_column_there * (star1_expansion_coefficient - 1) + dy + number_of_rows_there * (star1_expansion_coefficient - 1);
                    sum_s2 += dx + number_of_column_there * (star2_expansion_coefficient - 1) + dy + number_of_rows_there * (star2_expansion_coefficient - 1);
                }
            }
        }

        set_star1_result(sum_s1/2);
        set_star2_result(sum_s2/2);
    }
};