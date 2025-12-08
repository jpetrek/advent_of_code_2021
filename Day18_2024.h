#pragma once
#include "helper.h"
using namespace utility::io;
using namespace utility::algorithm;

template<>
class day<18, 2024> : public day_base<18, 2024>
{
    typedef point_2d_generic<long> coords;
    const std::vector<direction::name> where_to_go = { direction::up, direction::right, direction::down, direction::left };

    inline bool is_test() const override { return false; }

    graph_weighted<uint64_t> contruct_graph(const long max_x, const long max_y, const std::set<coords>& space) const
    {
        graph_weighted<uint64_t> gw(max_x * max_y);
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

    std::pair< uint64_t, bool> calculate_path(const long max_x, const long max_y, const std::set<coords>& space) const
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
        input_reader().read_by_line_until_condition_met_or_eof<size_t>(file_reader::empty_line_condition(), [&](const auto& line, const auto)
        {
            auto coords = split_and_convert<long>(line, ',');
            corrupted_bytes.push_back({ coords[0], coords[1] });
        });

        set_star1_result(dijkstra_shortest_path<uint64_t>(contruct_graph(max_x, max_y, construct_space(corrupted_bytes, how_many_star_1)), 0, max_y * max_x - 1).first);

        auto border = binary_search<size_t>(0, corrupted_bytes.size() - 1, [&](const auto middle)
        {
            return calculate_path(max_x, max_y, construct_space(corrupted_bytes, middle + 1)).second;
        });

        set_star2_result(std::to_string(corrupted_bytes[border.second].x) + "," + std::to_string(corrupted_bytes[border.second].y));
    }
};