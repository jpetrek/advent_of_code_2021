#pragma once
#include "helper.h"
using namespace utility::io;

template<>
class day<18, 2024> : public day_base<18, 2024>
{
    typedef point_2d_generic<long> coords;
    typedef direction_2d_generic<long>::diference increments;
    const std::vector<direction::name> where_to_go = { direction::up, direction::right, direction::down, direction::left };

    inline bool is_test() const override { return false; }

    graph_weighted contruct_graph(const long max_x, const long max_y, const std::set<coords>& space) const
    {
        graph_weighted gw(max_x * max_y);
        for_2D_rectangle_space<long>(0, max_x, 0, max_y, [&](auto x, auto y)
            {
                coords location = { x,y };
                if (!space.contains(location))
                {
                    for (auto w : where_to_go)
                    {
                        auto new_location = add(location, w);
                        if ((new_location.x >= 0) && (new_location.x < max_x) && (new_location.y >= 0) && (new_location.y < max_y) && (!space.contains(new_location)))
                        {
                            gw.add_twodirectional_edge(y * max_x + x, new_location.y * max_x + new_location.x, 1, 1);
                        }
                    }
                }
            });
        return gw;
    }

    size_t calculate_path(const long max_x, const long max_y, const std::set<coords>& space) const
    {
        auto graph = contruct_graph(max_x, max_y, space);
        return dijkstra_shortest_path(graph, 0, max_y * max_x - 1);
    }

    std::set<coords> construct_space(const std::vector<coords>& from, const size_t how_many) const
    {
        std::set<coords> ret;
        for (size_t i = 0; i < how_many; i++) ret.insert(from[i]);
        return ret;
    }

    void run_internal() override
    {
        long max_x = is_test() ? 7 : 71;
        long max_y = is_test() ? 7 : 71;
        long how_many_star_1 = is_test() ? 12 : 1024;

        std::vector<coords> corrupted_bytes;
        input_reader().read_by_line_until_condition_met_or_eof<size_t>(file_reader::read_condition::empty_line, [&](const auto& line, const auto)
        {
            auto coords = split_and_convert<long>(line, ',');
            corrupted_bytes.push_back({ coords[0], coords[1] });
        });

        set_star1_result(dijkstra_shortest_path(contruct_graph(max_x, max_y, construct_space(corrupted_bytes, how_many_star_1)), 0, max_y * max_x - 1));

        size_t left_index = 0;
        size_t right_index = corrupted_bytes.size() - 1;
        
        while ( (right_index - left_index) > 1 )
        {
            size_t half_index = (left_index + right_index) / 2;
            if (calculate_path(max_x, max_y, construct_space(corrupted_bytes, half_index + 1)) == std::numeric_limits<int64_t>::max())
            {
                right_index = half_index;
            }
            else
            {
                left_index = half_index;
            }
        }
        set_star2_result(std::to_string(corrupted_bytes[right_index].x) + "," + std::to_string(corrupted_bytes[right_index].y));
    }
};