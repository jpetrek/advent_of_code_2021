#pragma once
#include "helper.h"

template<>
class day<16, 2023> : public day_base<16, 2023>
{
    typedef point_2d_generic<long long> point_2d;
    
    struct beam
    {
        point_2d position;
        direction_2d ::name dir;
    };

    size_t calculate_energy(const std::vector<std::vector<char>>& map, const beam init_beam)
    {
        std::map<point_2d, direction_2d::name> visited;

        std::vector<beam> active_beams;
        active_beams.push_back(init_beam);

        while (active_beams.size() > 0)
        {
            std::vector<beam> new_active_beams;
            for (auto ab : active_beams)
            {
                auto dx = direction_2d::name_to_diff(ab.dir).dx;
                auto dy = direction_2d::name_to_diff(ab.dir).dy;
                
                if ((ab.position.x + dx >= 0) && (ab.position.x + dx < static_cast<point_2d::basic_type>(map[0].size())) &&
                    (ab.position.y + dy >= 0) && (ab.position.y + dy < static_cast<point_2d::basic_type>(map.size())))
                {
                    ab.position.x += dx;
                    ab.position.y += dy;

                    if (!visited.contains(ab.position) || visited.at(ab.position) != ab.dir)
                        {
                        visited[ab.position] = ab.dir;

                        if ((map[ab.position.y][ab.position.x] == '.')||
                            (map[ab.position.y][ab.position.x] == '-' && ((ab.dir == direction_2d::e) || (ab.dir == direction_2d::w)))||
                            (map[ab.position.y][ab.position.x] == '|' && ((ab.dir == direction_2d::n) || (ab.dir == direction_2d::s))))
                        {
                            new_active_beams.push_back(ab);
                        }

                        if (map[ab.position.y][ab.position.x] == '/')
                        {
                            if (ab.dir == direction_2d::e) ab.dir = direction_2d::n;
                            else if (ab.dir == direction_2d::w) ab.dir = direction_2d::s;
                            else if (ab.dir == direction_2d::n) ab.dir = direction_2d::e;
                            else if (ab.dir == direction_2d::s) ab.dir = direction_2d::w;
                            new_active_beams.push_back(ab);
                        }

                        if (map[ab.position.y][ab.position.x] == '\\')
                        {
                            if (ab.dir == direction_2d::e) ab.dir = direction_2d::south;
                            else if (ab.dir == direction_2d::w) ab.dir = direction_2d::n;
                            else if (ab.dir == direction_2d::n) ab.dir = direction_2d::w;
                            else if (ab.dir == direction_2d::s) ab.dir = direction_2d::e;
                            new_active_beams.push_back(ab);
                        }

                        if (map[ab.position.y][ab.position.x] == '-')
                        {
                            new_active_beams.push_back({ ab.position, direction_2d::e });
                            new_active_beams.push_back({ ab.position, direction_2d::w });
                        }

                        if (map[ab.position.y][ab.position.x] == '|')
                        {
                            new_active_beams.push_back({ ab.position, direction_2d::n });
                            new_active_beams.push_back({ ab.position, direction_2d::s });
                        }
                    }
                }
            }
            active_beams = new_active_beams;
        }
        return visited.size();
    }

    void run_internal() override
    {
        auto map = helper::transfor_input_into_char_array(input_reader());
        set_star1_result(calculate_energy(map, { {-1,0}, direction_2d::e}));

        min_max_counter<size_t> s2;
        for (long long y = 0; y < static_cast<long long>(map.size()); y++)
        {
            s2.check_value(calculate_energy(map, { {-1,y}, direction_2d::e }));
            s2.check_value(calculate_energy(map, { {static_cast<point_2d::basic_type>(map[0].size()),y}, direction_2d::w}));
        }

        for (long long x = 0; x < static_cast<long long>(map[0].size()); x++)
        {
            s2.check_value(calculate_energy(map, { {x,-1}, direction_2d::s}));
            s2.check_value(calculate_energy(map, { {x, static_cast<point_2d::basic_type>(map.size())}, direction_2d::n}));
        }
        set_star2_result(s2.maximum());
    }
};
