#pragma once
#include "helper.h"
using namespace utility::arrays;
using namespace utility::string;
using namespace utility::math;


template<>
class day<20, 2024> : public day_base<20, 2024>
{
    typedef std::vector<std::vector<char>> maze;
    typedef point_2d_generic<long> coords;
    typedef direction_2d_generic<long>::name dir;
    const std::vector<direction::name> directions = { direction::up, direction::right, direction::down, direction::left };

    size_t get_nr_of_shortcuts_over(const std::vector<coords>& path_points, size_t cheat_limit)
    {
        size_t sum = 0;
        for (long i = static_cast<long>(path_points.size()) - 1; i >= 0; i--)
        {
            for (long j = 0; j < path_points.size(); j++)
            {
                auto md = manhatan_distance(path_points[i], path_points[j]);
                if ((md <= cheat_limit) && ((j - i) > 0))
                {
                    if ((j - i - md) >= 100)
                    {
                        sum++;
                    }
                }
            }
        }
        return sum;
    }

    inline bool is_test() const override { return false; }

    void run_internal() override
    {
        maze race_track;
        coords start, finish;
        std::map<coords, long> distances_to_end;

        input_reader().read_by_line_until_condition_met_or_eof<long>(file_reader::empty_line_condition(), [&](const auto& line, const long y)
            {
                std::vector<char> lc;
                for (long x = 0; x < line.size(); x++)
                {
                    lc.push_back(line[x]);
                    if (line[x] == 'S') start = { x,y };
                    if (line[x] == 'E') finish = { x,y };
                }
                race_track.push_back(lc);
            });

        auto pos = finish;
        long dist = 0;
        std::vector<coords>path_points;
        while (pos != start)
        {
            path_points.push_back(pos);
            distances_to_end[pos] = dist;
            for (const auto d : directions)
            {
                auto new_pos = add(pos, d);
                if (is_inside_2D_array(race_track, new_pos) && !distances_to_end.contains(new_pos))
                {
                    if ((race_track.at(new_pos.y).at(new_pos.x) == '.') || (race_track.at(new_pos.y).at(new_pos.x) == 'S'))
                    {
                        pos = new_pos;
                        dist++;
                        break;
                    }
                }
            }
        }
        
        distances_to_end[pos] = dist; // add start;
        path_points.push_back(pos);

        set_star1_result(get_nr_of_shortcuts_over(path_points, 2));
        set_star2_result(get_nr_of_shortcuts_over(path_points, 20));
    }
};