#pragma once
#include "helper.h"

using namespace utility::arrays;
using namespace utility::geometry;
using namespace utility::math;
using namespace utility::string;

struct cube_set
{
    size_t red_cubes;
    size_t green_cubes;
    size_t blue_cubes;
};


struct game
{
    size_t id;
    std::vector<cube_set> sets;
};

game parse_input_line(const std::string& line)
{
    game g;
    auto game_strings = split(line, ':');
    
    auto id_strings = split(game_strings[0], ' ');
    g.id = std::stoul(id_strings[1]);
    
    auto set_strings = split(game_strings[1], ';');
    for (const auto& set_string : set_strings)
    {
        cube_set set{ 0,0,0 };
        auto color_strings = split(set_string, ',');
        for (const auto& color_string : color_strings)
        {
            auto number_color_string = split(color_string, ' ');
            if (number_color_string[1] == "red")
            {
                set.red_cubes = std::stoul(number_color_string[0]);
            }
            if (number_color_string[1] == "green")
            {
                set.green_cubes = std::stoul(number_color_string[0]);
            }
            if (number_color_string[1] == "blue")
            {
                set.blue_cubes = std::stoul(number_color_string[0]);
            }
        }
        g.sets.push_back(set);
    }
    return g;
}

template<>
class day<2, 2023> : public day_base<2,2023>
{
    void run_internal() override
    {
        std::vector<game> games;
        while (!input_reader().is_end_of_file())
        {
            games.push_back(parse_input_line(input_reader().get_line()));
        }

        size_t result_s1 = 0;
        size_t result_s2 = 0;
        for (const auto& g : games)
        {
            min_max_counter<size_t> r_cube_minmax;
            min_max_counter<size_t> g_cube_minmax;
            min_max_counter<size_t> b_cube_minmax;
            for (const auto& s : g.sets)
            {
                r_cube_minmax.check_value(s.red_cubes);
                g_cube_minmax.check_value(s.green_cubes);
                b_cube_minmax.check_value(s.blue_cubes);
            }
            if ((r_cube_minmax.maximum() <=12) && (g_cube_minmax.maximum() <= 13) && (b_cube_minmax.maximum() <= 14)) result_s1 += g.id;
            result_s2 += r_cube_minmax.maximum() * g_cube_minmax.maximum() * b_cube_minmax.maximum();
        }

        set_star1_result(result_s1);
        set_star2_result(result_s2);
    }
};