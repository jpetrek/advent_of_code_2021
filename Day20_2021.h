#pragma once
#include "helper.h"

template<>
class day<20, 2021> : public day_base<20, 2021>
{
    
    size_t get_binary(int64_t x, int64_t y, const std::map<std::pair<int64_t, int64_t>, bool>& field)
    {
        int64_t dx = -1;
        int64_t dy = -1;
        size_t index = 0;
        for (size_t i = 0; i < 9; i++)
        {
            index = index << 1;
            index += field.at({x + dx, y + dy});
            dx += 1;
            if (dx == 2)
            {
                dy += 1;
                dx = -1;
            }
        }
        return index;
    }

    void fill_borders_with_value(std::map<std::pair<int64_t, int64_t>, bool>& points, const  min_max_counter<int64_t>&x_limits, const  min_max_counter<int64_t>& y_limits, size_t n, bool value)
    {
        for (int64_t x = x_limits.minimum(); x <= x_limits.maximum(); x++)
        {
            for (int d = 0; d < n; d++)
            {
                points[{x, y_limits.minimum() + d}] = value;
                points[{x, y_limits.maximum() - d}] = value;
            }
        }
        
        for (int64_t y = y_limits.minimum(); y <= y_limits.maximum(); y++)
        {
            for (int d = 0; d < n; d++)
            {
                points[{x_limits.minimum() + d, y}] = value;
                points[{x_limits.maximum() - d, y}] = value;
            }
        }
    }


    void run_internal() override
    {
        
        min_max_counter<int64_t> x_limits;
        min_max_counter<int64_t> y_limits;

        std::vector<bool> kernel_3x3_value;
        std::map<std::pair<int64_t, int64_t>, bool> points;
        auto first_line = input_reader().get_line();
        for (auto c : first_line)
        {
            if (c == '.') kernel_3x3_value.push_back(false);
            if (c == '#') kernel_3x3_value.push_back(true);
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

        x_limits.adjust_limits(x_limits.minimum() - 1, x_limits.maximum() + 1);
        y_limits.adjust_limits(y_limits.minimum() - 1, y_limits.maximum() + 1);
        fill_borders_with_value(points, x_limits, y_limits, 1, false);

        for (size_t i = 0; i < 50; i++)
        {
            x_limits.adjust_limits(x_limits.minimum() -3, x_limits.maximum() +3);
            y_limits.adjust_limits(y_limits.minimum() - 3, y_limits.maximum() + 3);
            
            fill_borders_with_value(points, x_limits, y_limits, 3, points[{x_limits.minimum() + 3, y_limits.minimum() + 3}]);
            
            x_limits.adjust_limits(x_limits.minimum() + 1, x_limits.maximum() - 1);
            y_limits.adjust_limits(y_limits.minimum() + 1, y_limits.maximum() - 1);

            std::map<std::pair<int64_t, int64_t>, bool>  points_new;

            for (int64_t y = y_limits.minimum(); y <= y_limits.maximum();y++)
                for (int64_t x = x_limits.minimum(); x <= x_limits.maximum(); x++)
                {
                    points_new[{x, y}] = kernel_3x3_value[get_binary(x, y, points)];
                }

            points = points_new;

            if (i == 1)
            {
                set_star1_result(std::accumulate(std::cbegin(points), std::cend(points), static_cast<size_t>(0), [](size_t r, const auto& p) {return p.second ? r += 1 : r; }));
            }

            std::cout << i << std::endl;
        }

        set_star2_result(std::accumulate(std::cbegin(points), std::cend(points), static_cast<size_t>(0), [](size_t r, const auto& p) {return p.second ? r += 1 : r; }));
    }
};
