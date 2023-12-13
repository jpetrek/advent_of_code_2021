#pragma once
#include "helper.h"

template<>
class day<10, 2023> : public day_base<10,2023>
{
    enum direction
    {
        right = 0,
        left = 1,
        up = 2,
        down = 3
    };

    std::tuple<size_t, size_t, direction, bool> go(std::tuple<size_t, size_t, direction> actual, const size_t w, const size_t h, const std::map<std::pair<size_t, size_t>, char>& map)
    {
        auto [x, y, d] = actual;
        if (d == direction::right) return go_right(actual, w, h, map);
        if (d == direction::left) return go_left (actual, w, h, map);
        if (d == direction::up) return go_up(actual, w, h, map);
        if (d == direction::down) return go_down(actual, w, h, map);
    }

    std::tuple<size_t, size_t, direction, bool> go_up(std::tuple<size_t, size_t, direction> actual, const size_t w, const size_t h, const std::map<std::pair<size_t, size_t>, char>& map)
    {
        auto [x, y, d] = actual;
        if (y == 0) return { x,y,d,false };
        auto c = map.at({ x,y - 1 });
        if (c == '|') return { x,y - 1, d, true };
        if (c == '7') return { x,y - 1, direction::left, true };
        if (c == 'F') return { x,y - 1, direction::right, true };
        return { x,y-1,d,false };
    }

    std::tuple<size_t, size_t, direction, bool> go_down(std::tuple<size_t, size_t, direction> actual, const size_t w, const size_t h, const std::map<std::pair<size_t, size_t>, char>& map)
    {
        auto [x, y, d] = actual;
        if (y == (h-1)) return { x,y,d,false };
        auto c = map.at({ x,y + 1 });
        if (c == '|') return { x,y + 1, d, true };
        if (c == 'J') return { x,y + 1, direction::left, true };
        if (c == 'L') return { x,y + 1, direction::right, true };
        return { x,y+1,d,false };
    }

    std::tuple<size_t, size_t, direction, bool> go_right(std::tuple<size_t, size_t, direction> actual, const size_t w, const size_t h, const std::map<std::pair<size_t, size_t>, char>& map)
    {
        auto [x, y, d] = actual;
        if (x == (w-1)) return { x,y,d,false };
        auto c = map.at({ x+1,y});
        if (c == '-') return { x+1,y, d, true };
        if (c == '7') return { x+1,y, direction::down, true };
        if (c == 'J') return { x+1,y, direction::up, true };
        return { x+1,y,d,false };
    }

    std::tuple<size_t, size_t, direction, bool> go_left(std::tuple<size_t, size_t, direction> actual, const size_t w, const size_t h, const std::map<std::pair<size_t, size_t>, char>& map)
    {
        auto [x, y, d] = actual;
        if (x == 0) return { x,y,d,false };
        auto c = map.at({ x - 1,y });
        if (c == '-') return { x - 1,y, d, true };
        if (c == 'F') return { x - 1,y, direction::down, true };
        if (c == 'L') return { x - 1,y, direction::up, true };
        return { x-1,y,d,false };
    }
    
    std::pair<char, direction> get_real_start(const std::map < std::pair<size_t, size_t>, char>& map, std::pair<size_t, size_t> s, size_t w, size_t h)
    {
        std::vector<direction> possible_starts;
        for (const auto d : {direction::right, direction::left, direction::up, direction::down })
        {
            auto [x, y, nd, was_move] = go({ s.first, s.second, d }, w, h, map);
            if (was_move)possible_starts.push_back(d);
        }

        if ((possible_starts[0] == direction::right) && (possible_starts[1] == direction::left)) return { '-', direction::right };
        if ((possible_starts[0] == direction::right) && (possible_starts[1] == direction::up)) return {'L', direction::right };
        if ((possible_starts[0] == direction::right) && (possible_starts[1] == direction::down)) return {'F', direction::right };
        if ((possible_starts[0] == direction::left) && (possible_starts[1] == direction::up)) return { 'J', direction::left };
        if ((possible_starts[0] == direction::left) && (possible_starts[1] == direction::down)) return { '7', direction::left };
        if ((possible_starts[0] == direction::up) && (possible_starts[1] == direction::down)) return { '|', direction::up };
    }

    std::map<std::pair<size_t, size_t>, char> transform_to_star2(const std::map<std::pair<size_t, size_t>, char>& map, size_t w, size_t h, const std::set<std::pair<size_t, size_t>>& cycle_parts)
    {
        // remove all non-cycle characters and replace it by .
        // also identify F----J (bottom->up path) like parts and replace it by F----| to easily count parity of | later
        // also identify L----7 (top->bottom path) like parts and replace it by L----| to easily count parity of | later
        std::map<std::pair<size_t, size_t>, char> ret;
        for (size_t y = 0; y < h; y++)
        {
            for (size_t x = 0; x < w; x++)
            {
                if (cycle_parts.contains({ x,y }))
                {
                    auto c = map.at({ x,y });
                    ret[{x, y}] = c;
                    if (c == 'F')
                    {
                        x++;
                        while (x < w && map.at({ x,y }) == '-')
                        {
                            ret[{x, y}] = '-';
                            x++;
                        }
                        if ((x < w) && map.at({ x,y }) == 'J')
                        {
                            ret[{x, y}] = '|';
                        }
                        else if(x < w)
                        {
                            ret[{x, y}] = map.at({x, y});
                        }
                    }
                    if (c == 'L')
                    {
                        x++;
                        while (x < w && map.at({ x,y }) == '-')
                        {
                            ret[{x, y}] = '-';
                            x++;
                        }
                        if ((x < w) && map.at({ x,y }) == '7')
                        {
                            ret[{x, y}] = '|';
                        }
                        else if (x < w)
                        {
                            ret[{x, y}] = map.at({ x, y });
                        }
                    }
                }
                else
                {
                    ret[{x, y}] = '.';
                }
            }
        }
        return ret;
    }

    void run_internal() override
    {
        //set_is_test(true);
        std::vector<direction> dirs = { direction::right, direction::left,direction::up,direction::down };
        std::pair<size_t, size_t> start = { 0,0 };
        std::map<std::pair<size_t, size_t>, char> map;
        std::map<std::pair<size_t, size_t>, char> bigger_map;
        size_t w = 0;
        size_t h = 0;
        
        while (!input_reader().is_end_of_file())
        {
            auto l = input_reader().get_line();
            w = l.size();
            for (size_t i = 0; i < l.size(); i++)
            {
                map[{i, h}] = l[i];
                if (l[i] == 'S') start = { i,h };
            }
            h++;
        }

        min_max_counter<size_t> s1;
        std::set<std::pair<size_t, size_t>> cycle_parts;
        
        auto [S, start_direction] = get_real_start(map, start, w, h);
        map[start] = S;

        size_t ns = 0;
        auto act_pos = start;
        direction act_dir = start_direction;
        while (true)
        {
            cycle_parts.insert(act_pos);
            auto [x, y, nd, was_move] = go({ act_pos.first, act_pos.second, act_dir }, w, h, map);
            if (x == start.first && y == start.second)
            {
                s1.check_value(static_cast<size_t>(std::ceil(ns / 2.0)));
                break;
            }
            if (!was_move)
            {
                break;
            }
            ns++;
            act_pos = { x,y };
            act_dir = nd;
        }

        set_star1_result(s1.maximum());
 
        auto m2 = transform_to_star2(map, w, h, cycle_parts);
        
        size_t counter = 0;
        for (size_t i = 0; i < h; i++)
        {
            size_t nob = 0;
            for (size_t j = 0; j < w; j++)
            {
                auto c = m2.at({ j,i });
                if (c == '|') nob++;
                else if (c =='.')
                {
                    if (nob % 2 == 1)
                    {
                        counter++;
                    }
                }
            }
        }
        set_star2_result(counter);
    }
};