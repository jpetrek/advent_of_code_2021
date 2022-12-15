#pragma once
#include "helper.h"

template<>
class day<9, 2022> : public day_base<9,2022>
{
    std::pair<int, int> get_new_position(const std::pair<int, int>& h, const std::pair<int, int>& t)
    {
        std::pair<int, int> result = t;
        if (abs(h.first - t.first) > 1 || abs(h.second - t.second) > 1)
        {
            if (h.first > t.first) result.first++;
            if (h.first < t.first) result.first--;
            if (h.second > t.second) result.second++;
            if (h.second < t.second) result.second--;
        }
        return result;
    }

    void run_internal() override
    {
        std::map<std::string, std::pair<int, int>> directions = { {"U",{0,-1}},{"D",{0,1}},{"L",{-1,0}},{"R",{1,0}} };
        std::vector<std::pair<int, int>> snake_star1(2, { 0,0 });
        std::vector<std::pair<int, int>> snake_star2(10, { 0,0 });
        std::set<std::pair<int, int>> visited_star1 = { snake_star1[0] };
        std::set<std::pair<int, int>> visited_star2 = { snake_star2[0] };
        while (!input_reader().is_end_of_file())
        {
            auto in = helper::split(input_reader().get_line(), ' ');
            for (size_t step = 0; step < std::stoul(in[1]); step++)
            {
                snake_star1[0].first += directions[in[0]].first;
                snake_star1[0].second += directions[in[0]].second;
                for (size_t i = 1; i < snake_star1.size(); i++)
                {
                    snake_star1[i] = get_new_position(snake_star1[i - 1], snake_star1[i]);
                }
                visited_star1.insert(snake_star1.back());

                snake_star2[0].first += directions[in[0]].first;
                snake_star2[0].second += directions[in[0]].second;
                for (size_t i = 1; i < snake_star2.size(); i++)
                {
                    snake_star2[i] = get_new_position(snake_star2[i - 1], snake_star2[i]);
                }
                visited_star2.insert(snake_star2.back());
            }
        }
        set_star1_result(visited_star1.size());
        set_star2_result(visited_star2.size());
    }
};