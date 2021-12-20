#pragma once
#include "helper.h"

class day20 : public day_base<20>
{
    
    size_t get_binary(int64_t x, int64_t y, std::map<std::pair<int64_t, int64_t>, bool>& field)
    {
        int64_t dx = -1;
        int64_t dy = -1;
        size_t index = 0;
        for (size_t i = 0; i < 9; i++)
        {
            index = index << 1;
            index += field[{x + dx, y + dy}];
            dx += 1;
            if (dx == 2)
            {
                dy += 1;
                dx = -1;
            }
        }
        return index;
    }

    void run_interal() override
    {
        
        min_max_counter<int64_t> x_limits;
        min_max_counter<int64_t> y_limits;

        std::vector<bool> mapping;
        std::map<std::pair<int64_t, int64_t>, bool> points;
        auto first_line = input_reader().get_line();
        for (auto c : first_line)
        {
            if (c == '.') mapping.push_back(false);
            if (c == '#') mapping.push_back(true);
        }

        auto empty_line = input_reader().get_line();
        
        int64_t y = 0;
        while (!input_reader().is_end_of_file())
        {
            y_limits.check_value(y);
            auto line = input_reader().get_line();
            for (size_t x = 0; x < line.size(); x++)
            {
                x_limits.check_value(x);
                if (line[x] == '.') points[{x, y}] = false;
                if (line[x] == '#') points[{x, y}] = true;
            }
            y++;
        }

        // initial grow by zeros
        x_limits.check_value(x_limits.minimum() - 1);
        x_limits.check_value(x_limits.maximum() + 1);
        y_limits.check_value(y_limits.minimum() - 1);
        y_limits.check_value(y_limits.maximum() + 1);

        for (int64_t x = x_limits.minimum(); x <= x_limits.maximum(); x++)
        {
            points[{x, y_limits.minimum()}] = false;
            points[{x, y_limits.maximum()}] = false;
        }

        for (int64_t y = y_limits.minimum(); y <= y_limits.maximum(); y++)
        {
            points[{x_limits.minimum(), y}] = false;
            points[{x_limits.maximum(), y}] = false;
        }


        for (size_t i = 0; i < 50; i++)
        {
            auto x_l_cbk = x_limits;
            auto y_l_cbk = y_limits;

            x_limits.check_value(x_limits.minimum() - 3);
            x_limits.check_value(x_limits.maximum() + 3);
            y_limits.check_value(y_limits.minimum() - 3);
            y_limits.check_value(y_limits.maximum() + 3);
            
            for (int64_t x = x_limits.minimum(); x <= x_limits.maximum(); x++)
            {
                for (int d = 0; d < 3; d++)
                {
                    points[{x, y_limits.minimum() + d}] = points[{x_limits.minimum() + 3, y_limits.minimum() + 3}];
                    points[{x, y_limits.maximum() - d}] = points[{x_limits.minimum() + 3, y_limits.minimum() + 3}];
                }
            }

            for (int64_t y = y_limits.minimum(); y <= y_limits.maximum(); y++)
            {
                for (int d = 0; d < 3; d++)
                {
                    points[{x_limits.minimum()+ d, y}] = points[{x_limits.minimum() + 3, y_limits.minimum() + 3}];
                    points[{x_limits.maximum() - d, y}] = points[{x_limits.minimum() + 3, y_limits.minimum() + 3}];
                }
            }

            std::map<std::pair<int64_t, int64_t>, bool>  points_new;

            for (int64_t y = y_limits.minimum() +1; y <= y_limits.maximum()-1;y++)
                for (int64_t x = x_limits.minimum() +1; x <= x_limits.maximum()-1; x++)
                {
                    auto i = get_binary(x, y, points);
                    points_new[{x, y}] = mapping[i];
                }

            points = points_new;

            x_limits = x_l_cbk;
            y_limits = y_l_cbk;
            x_limits.check_value(x_limits.minimum() - 2);
            x_limits.check_value(x_limits.maximum() + 2);
            y_limits.check_value(y_limits.minimum() - 2);
            y_limits.check_value(y_limits.maximum() + 2);

            if (i == 1)
            {
                size_t sum = 0;
                for (const auto& p : points) if (p.second) sum++;
                set_star1_result(sum);
            }

            std::cout << i << std::endl;
        }

        size_t sum = 0;
        for (const auto& p : points) if (p.second) sum++;
        set_star2_result(sum);
    }
};
