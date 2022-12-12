#pragma once
#include "helper.h"

template<>
class day<12, 2022> : public day_base<12,2022>
{
    void next_step(const std::vector<std::vector<char>>& map, std::pair<size_t, size_t> current, size_t current_length, std::map<std::pair<size_t, size_t>, size_t>& path_cache, size_t star)
    {
        if (path_cache.contains(current))
        {
            if (path_cache[current] <= current_length) return;
        }

        path_cache[current] = current_length;

        helper::do_for_adjacent(map[0].size(), map.size(), current.first, current.second, { {1, 0}, {0,1}, {-1, 0}, {0, -1} }, [&](auto x, auto y)
            {
                if (star == 1)
                {
                    if (((map[y][x]) == (map[current.second][current.first] + 1)) || ((map[y][x] <= map[current.second][current.first])))
                    {
                        next_step(map, { x, y }, current_length + 1, path_cache, star);
                    }
                }
                else
                {
                    if (((map[y][x]) == (map[current.second][current.first] - 1)) || ((map[y][x] >= map[current.second][current.first])))
                    {
                        next_step(map, { x, y }, current_length + 1, path_cache, star);
                    }
                }
            });
    }

    void run_interal() override
    {
        std::vector<std::vector<char>> hieght_map;
        std::pair<size_t, size_t> start_position;
        std::pair<size_t, size_t> end_postion;

        while (!input_reader().is_end_of_file())
        {
            std::vector<char> l;
            auto line = input_reader().get_line();
            for (size_t i = 0; i < line.size(); i++)
            {
                if (line[i] == 'S')
                {
                    start_position = { i, hieght_map.size()};
                }
                if (line[i] == 'E')
                {
                    end_postion = { i, hieght_map.size() };
                }
                l.push_back(line[i]);
            }
            hieght_map.push_back(l);
        }

        hieght_map[start_position.second][start_position.first] = 'a';
        hieght_map[end_postion.second][end_postion.first] = 'z';

        std::map<std::pair<size_t, size_t>, size_t> path_cache;
        next_step(hieght_map, start_position, 0, path_cache, 1);
        set_star1_result(path_cache.at(end_postion));

        path_cache.clear();
        next_step(hieght_map, end_postion, 0,path_cache, 2);

        min_max_counter<size_t> m;
        for (const auto& c : path_cache)
        {
            if (hieght_map[c.first.second][c.first.first] == 'a') m.check_value(c.second);
        }
        
        set_star2_result(m.minimum());
    }
};