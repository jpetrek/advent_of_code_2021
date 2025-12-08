#pragma once
#include "helper.h"

template<>
class day<4, 2025> : public day_base<4,2025>
{
    typedef point_2d_generic<size_t> point_2d;
    const char paper_roll = '@';
    const char empty_space = '.';

    inline bool is_test() const override { return false; }
    std::vector<point_2d> get_forklifts(const std::vector<std::vector<char>>& data)
    {
        std::vector<point_2d> result;
        for_2D_rectangle_space<size_t>(0, data[0].size(), 0, data.size(), [&](auto x, auto y)
        {
            if (data[y][x] == paper_roll)
            {
                size_t number_of_rolls_around = 0;
                do_for_adjacent(data[0].size(), data.size(), x, y, [&](size_t a, size_t b)
                    {
                        if (data[b][a] == paper_roll) number_of_rolls_around++;
                    });
                if (number_of_rolls_around < 4)
                {
                    result.push_back(point_2d(x, y));
                }
            }
        });
        return result;
    }
    
    void run_internal() override
    {
        size_t sum_1 = 0;
        size_t sum_2 = 0;

        std::vector<std::vector<char>> data;

        while (!input_reader().is_end_of_file())
        {
            auto l = input_reader().get_line();
            data.push_back(std::vector<char>(l.begin(), l.end()));
        }
        auto forklifts = get_forklifts(data);
        sum_1 = forklifts.size();
        while (forklifts.size() > 0)
        {
            sum_2 += forklifts.size();
            for (const auto& f : forklifts)
            {
                data[f.y][f.x] = empty_space;
            }
            forklifts = get_forklifts(data);
        }

        set_star1_result(sum_1);
        set_star2_result(sum_2);
    }
};