#pragma once
#include "helper.h"

template<>
class day<10, 2024> : public day_base<10,2024>
{
    typedef std::vector<std::vector<long>> height_map;
    typedef std::set<point_2d> unique_points;
    typedef std::set<std::vector<point_2d>> unique_routes;

    const std::vector<direction::name> where_to_go =
    {
        direction::up, direction::right, direction::down, direction::left
    };
    
    void traverse(unique_routes& routes_to_peak, unique_points& peaks, std::vector<point_2d> current_path, const height_map& hmap)
    {
        if (get_value(hmap, current_path.back()) == 9)
        {
            routes_to_peak.insert(current_path);
            peaks.insert(current_path.back());
            return;
        }

        for (auto wt : where_to_go)
        {
            auto pos = add(current_path.back(), wt);
            if (is_inside_2D_array(hmap, pos))
            {
                if (get_value(hmap, pos) == get_value(hmap, current_path.back()) + 1)
                {
                    current_path.push_back(pos);
                    traverse(routes_to_peak, peaks, current_path, hmap);
                    current_path.erase(current_path.end() - 1);
                }
            }
        }
    }

    inline bool is_test() const override { return false; }
    void run_internal() override
    {
        height_map space = utility::io::transform_input_into_array<long>(input_reader(), [](auto c) {return c - '0'; });
        size_t sum1 = 0;
        size_t sum2 = 0;

        for (int y = 0; y < space.size(); y++)
        {
            for (int x = 0; x < space[0].size(); x++)
            {
                if (space[y][x] == 0)
                {
                    unique_points peaks;
                    unique_routes routes_to_peak;
                    traverse(routes_to_peak, peaks, { {x,y} }, space);
                    sum1 += peaks.size();
                    sum2 += routes_to_peak.size();
                }
            }
        }

        set_star1_result(sum1);
        set_star2_result(sum2);
    }
};