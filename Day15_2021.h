#pragma once
#include "helper.h"

using namespace utility::arrays;
using namespace utility::geometry;
using namespace utility::math;
using namespace utility::string;

template<>
class day<15, 2021> : public day_base<15, 2021>
{
    graph_weighted<uint64_t> transform_maze_to_graph(const std::vector<std::vector<size_t>>& maze)
    {
        graph_weighted<uint64_t> g(maze.size() * maze[0].size());
        for (size_t y = 0; y < maze.size(); y++)
            for (size_t x = 0; x < maze[0].size(); x++)
            {
                size_t ei = y * maze[0].size() + x;
                if (y > 0)  g.add_twodirectional_edge(ei, (y - 1) * maze[0].size() + x, maze[y - 1][x], maze[y][x]);
                if (y < maze.size() - 1)  g.add_twodirectional_edge(ei, (y + 1) * maze[0].size() + x, maze[y + 1][x], maze[y][x]);
                if (x > 0)  g.add_twodirectional_edge(ei, (y)*maze[0].size() + (x - 1), maze[y][x - 1], maze[y][x]);
                if (x < maze[0].size() - 1)  g.add_twodirectional_edge(ei, (y)*maze[0].size() + (x + 1), maze[y][x + 1], maze[y][x]);
            }
        return g;
    }

    void run_internal() override
    {
        std::vector<std::vector<size_t>> maze;
        while (!input_reader().is_end_of_file())
        {
            maze.push_back(convert_string_of_digits_to_vector<size_t>(input_reader().get_line()));
        }

        std::vector<std::vector<size_t>> maze_huge;
        for (size_t y = 0; y < maze.size() * 5; y++)
        {
            std::vector<size_t> row;
            for (size_t x = 0; x < maze[0].size() * 5; x++)
            {
                size_t dx = x / maze[0].size();
                size_t dy = y / maze.size();
                size_t new_val = (maze[y % maze.size()][x % maze[0].size()] + dx + dy);
                while (new_val > 9) new_val -= 9;
                row.push_back(new_val);
            }
            maze_huge.push_back(row);
        }

        auto graph_maze = transform_maze_to_graph(maze);
        set_star1_result(utility::algorithm::dijkstra_shortest_path<uint64_t>(graph_maze, 0, graph_maze.count() - 1).first);

        auto graph_maze_huge = transform_maze_to_graph(maze_huge);
        set_star2_result(utility::algorithm::dijkstra_shortest_path<uint64_t>(graph_maze_huge, 0, graph_maze_huge.count() - 1).first);
    }
};
