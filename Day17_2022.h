#pragma once
#include "helper.h"

template<>
class day<17, 2022> : public day_base<17, 2022>
{
    typedef std::vector<std::vector<int>> shape;

    const shape horizontal_line =
    {
        {1,1,1,1}
    };

    const shape cross =
    {
         {0,1,0},
         {1,1,1},
         {0,1,0}
    };

    const shape el =
    {
     {0,0,1},
     {0,0,1},
     {1,1,1}
    };

    const shape vertical_line =
    {
     {1},
     {1},
     {1},
     {1}
    };

    const shape rect =
    {
        {1,1},
        {1,1}
    };

    std::pair<long, long> limits(const shape& s)
    {
        min_max_counter<long> l_x;
        min_max_counter<long> l_y;

        for (long y = 0; y < s.size(); y++)
        {
            for (long x = 0; x < s[0].size(); x++)
            {
                if (s[y][x] == 1)
                {
                    l_x.check_value(x);
                    l_y.check_value(y);
                }
            }
        }
        return { l_x.maximum(), l_y.maximum() };
    }

    long get_max_y(const std::map <std::pair<int, int>, bool>& cave)
    {
        min_max_counter<long> l_y;
        l_y.check_value(-1);
        for (auto& c : cave)
        {
            l_y.check_value(c.first.second);
        }
        return l_y.maximum();
    }

    bool is_free(long global_x, long global_y, const shape& s, const std::map <std::pair<int, int>, bool>& cave)
    {
        for (long y = 0; y < s.size(); y++)
        {
            for (long x = 0; x < s[0].size(); x++)
            {
                if (s[y][x] == 1)
                {
                    if ((global_x + x > 6) || (global_x + x < 0) || (global_y - y < 0))  return false;
                    if (cave.contains({ global_x + x , global_y - y })) return false;
                }
            }
        }
        return true;
    }

    bool fall_by_one(const long global_x, long& global_y, const shape& s, std::map <std::pair<int, int>, bool>& cave)
    {
        if (is_free(global_x, global_y - 1, s, cave))
        {
            global_y--;
            return true;
        }
        else
        {
            for (long y = 0; y < s.size(); y++)
            {
                for (long x = 0; x < s[0].size(); x++)
                {
                    if (s[y][x] == 1)
                    {
                        cave[{global_x + x, global_y - y}] = true;
                    }
                }
            }
        }
        return false;
    }

    void modify_position_by_input(long& g_x, long g_y, char d, const shape& s, const std::map <std::pair<int, int>, bool>& cave)
    {
        if (d == '<')
        {
            if (is_free(g_x-1, g_y, s, cave )) g_x--;
        }
        if (d == '>')
        {
            if (is_free(g_x + 1, g_y, s, cave)) g_x++;
        }
    }

    size_t calculate_last_line_state(const std::map <std::pair<int, int>, bool>& cave)
    {
        auto y = get_max_y(cave);
        size_t ret = 0;
        for (long x = 0; x < 7; x++)
        {
            if (cave.contains({ x,y })) ret = ret + 1;
            ret = ret << 1;
        }
        return ret;
    }

    size_t process(std::map <std::pair<int, int>, bool> cave, const std::vector<shape>&shapes, const std::string& input, size_t max_rock_count, size_t i_init = 0, size_t si_init = 0)
    {
        long sum = 0;
        size_t si = si_init;
        size_t ri = 0;
        size_t i = i_init;
        
        std::map<size_t, size_t> hit_cache;
        long repeat_key = -1;
        size_t ri_stored = 0;
        while (ri < max_rock_count)
        {
            ri++;
            auto actual_shape = shapes[si++];
            if (si == shapes.size()) si = 0;
            long g_x = 2;
            long g_y = get_max_y(cave) + 4 + limits(actual_shape).second;
            
            bool moving = true;
            while (moving)
            {
                if (i == input.size()) i = 0;
                modify_position_by_input(g_x, g_y, input[i++], actual_shape, cave);
                moving = fall_by_one(g_x, g_y, actual_shape, cave);
            }

            size_t state = calculate_last_line_state(cave);
            size_t key = state + (si << 8) + (i << 16);

            if (hit_cache.contains(key))
            {
                if (repeat_key != -1)
                {
                    if (key == repeat_key)
                    {
                        size_t init = hit_cache[key];
                        size_t period = ri - ri_stored;
                        size_t multiple = (get_max_y(cave) + static_cast<size_t>(1) - hit_cache[key]) / 2;
                        size_t m = (static_cast<size_t>(1) +((max_rock_count - ri_stored)) / period) * multiple;
                        size_t rr = (((max_rock_count - ri_stored)) % period);
                        size_t rs = process(cave, shapes, input, rr, i, si) - (get_max_y(cave) + static_cast<size_t>(1));
                        size_t ss = init + m + rs;
                        return ss;
                    }
                }
                else
                {
                    repeat_key = static_cast<long>(key);
                    ri_stored = ri;
                }
            }
            else
            {
                hit_cache[key] = get_max_y(cave) + static_cast<size_t>(1);
            }
        }
        return get_max_y(cave) + +static_cast<size_t>(1);
    }

    void run_internal() override
    {
        std::vector<shape> shapes = { horizontal_line, cross, el, vertical_line, rect };
        while (!input_reader().is_end_of_file())
        {
            auto line = input_reader().get_line();
            set_star1_result(process(std::map <std::pair<int, int>, bool>(), shapes, line, 2022));
            set_star2_result(process(std::map <std::pair<int, int>, bool>(), shapes, line, 1000000000000));
        }
    }
};
