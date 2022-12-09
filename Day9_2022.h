#pragma once
#include "helper.h"

template<>
class day<9, 2022> : public day_base<9,2022>
{
    std::pair<int, int> get_new_position(const std::pair<int, int>& h, const std::pair<int, int>& t)
    {
        std::pair<int, int> Tp = t;
        if (abs(h.first - t.first) > 1 || abs(h.second - t.second) > 1)
        {
            if (h.first > t.first) Tp.first++;
            if (h.first < t.first) Tp.first--;
            if (h.second > t.second) Tp.second++;
            if (h.second < t.second) Tp.second--;
        }
        return Tp;
    }

    void run_interal() override
    {
        std::vector<std::pair<int, int>> snake2(2, { 0,0 });
        std::vector<std::pair<int, int>> snake10(10, { 0,0 });
        std::set<std::pair<int, int>> visited2 = { snake2[0] };
        std::set<std::pair<int, int>> visited10 = { snake10[0] };
        while (!input_reader().is_end_of_file())
        {
            auto in = helper::split(input_reader().get_line(), ' ');
            auto r = std::stoul(in[1]);
            for (size_t s = 0; s < r; s++)
            {
                if (in[0] == "U")
                {
                    snake2[0].second--;
                    snake10[0].second--;
                }
                if (in[0] == "D")
                {
                    snake2[0].second++;
                    snake10[0].second++;
                }
                if (in[0] == "L")
                {
                    snake2[0].first--;
                    snake10[0].first--;
                }
                if (in[0] == "R")
                {
                    snake2[0].first++;
                    snake10[0].first++;
                }
                    
                for (size_t i = 1; i < snake2.size(); i++)
                {
                    snake2[i] = get_new_position(snake2[i - 1], snake2[i]);
                }
                visited2.insert(snake2.back());

                for (size_t i = 1; i < snake10.size(); i++)
                {
                    snake10[i] = get_new_position(snake10[i - 1], snake10[i]);
                }
                visited10.insert(snake10.back());
            }
        }
        set_star1_result(visited2.size());
        set_star2_result(visited10.size());
    }
};