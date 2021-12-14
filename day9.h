#pragma once
#include "helper.h"

class day9 : public day_base<9>
{
    enum class directions
    {
        none,
        left,
        right,
        up,
        down
    };

    void calculate_basin(const std::vector<std::vector<size_t>>& data, size_t x, size_t y, size_t last_value, directions last_move_direction, std::set<size_t>& visited)
    {
        if (data[y][x] == 9) return;

        if ((data[y][x] > last_value) || (last_move_direction == directions::none))
        {
            visited.insert(x + y * data[0].size());
            if ((x > 0) && (last_move_direction != directions::right)) calculate_basin(data, x - 1, y, data[y][x], directions::left, visited);
            if ((x < data[y].size() - 1) && (last_move_direction != directions::left)) calculate_basin(data, x + 1, y, data[y][x], directions::right, visited);
            if ((y > 0) && (last_move_direction != directions::down)) calculate_basin(data, x, y - 1, data[y][x], directions::up, visited);
            if ((y < data.size() - 1) && (last_move_direction != directions::up)) calculate_basin(data, x, y + 1, data[y][x], directions::down, visited);
        }
    }
    
    void run_interal() override
    {
        std::vector<std::vector<size_t>> heights;
        while (!input_reader().is_end_of_file())
        {
            std::vector<size_t> values;
            auto l = input_reader().get_line();
            for (auto c : l)
            {
                values.push_back(static_cast<size_t>(c) - static_cast<size_t>('0'));
            }
            heights.push_back(values);
        }

        size_t sum = 0;
        std::vector<size_t> basins;
        for (size_t y = 0; y < heights.size(); y++)
        {
            for (size_t x = 0; x < heights[y].size(); x++)
            {
                size_t left = helper::get_value_safe<size_t>(heights, x, y, -1, 0, 9);
                size_t right = helper::get_value_safe<size_t>(heights, x, y, 1, 0, 9);
                size_t up = helper::get_value_safe<size_t>(heights, x, y, 0, -1, 9);
                size_t down = helper::get_value_safe<size_t>(heights, x, y, 0, 1, 9);

                if ((heights[y][x] < left) && (heights[y][x] < right) && (heights[y][x] < up) && (heights[y][x] < down))
                {
                    sum += heights[y][x] + 1;
                }

                std::set<size_t> visited;
                calculate_basin(heights, x, y, 0, directions::none, visited);
                basins.push_back(visited.size());
            }
        }

        set_star1_result(sum);

        std::sort(basins.rbegin(), basins.rend());
        set_star2_result(basins[0] * basins[1] * basins[2]);
    }
};
