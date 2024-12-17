#pragma once
#include "helper.h"

template<>
class day<16, 2023> : public day_base<16, 2023>
{
    typedef point_2d_generic<long long> point_2d;
    
    struct beam
    {
        point_2d position;
        direction ::name dir;
    };

    size_t calculate_energy(const std::vector<std::vector<char>>& map, const beam init_beam)
    {
        std::map<point_2d, direction::name> visited;

        std::vector<beam> active_beams;
        active_beams.push_back(init_beam);

        while (active_beams.size() > 0)
        {
            std::vector<beam> new_active_beams;
            for (auto ab : active_beams)
            {
                auto dx = direction::name_to_diff(ab.dir).dx;
                auto dy = direction::name_to_diff(ab.dir).dy;
                
                if ((ab.position.x + dx >= 0) && (ab.position.x + dx < static_cast<point_2d::basic_type>(map[0].size())) &&
                    (ab.position.y + dy >= 0) && (ab.position.y + dy < static_cast<point_2d::basic_type>(map.size())))
                {
                    ab.position.x += dx;
                    ab.position.y += dy;

                    if (!visited.contains(ab.position) || visited.at(ab.position) != ab.dir)
                        {
                        visited[ab.position] = ab.dir;

                        if ((map[ab.position.y][ab.position.x] == '.')||
                            (map[ab.position.y][ab.position.x] == '-' && ((ab.dir == direction::e) || (ab.dir == direction::w)))||
                            (map[ab.position.y][ab.position.x] == '|' && ((ab.dir == direction::n) || (ab.dir == direction::s))))
                        {
                            new_active_beams.push_back(ab);
                        }

                        if (map[ab.position.y][ab.position.x] == '/')
                        {
                            if (ab.dir == direction::e) ab.dir = direction::n;
                            else if (ab.dir == direction::w) ab.dir = direction::s;
                            else if (ab.dir == direction::n) ab.dir = direction::e;
                            else if (ab.dir == direction::s) ab.dir = direction::w;
                            new_active_beams.push_back(ab);
                        }

                        if (map[ab.position.y][ab.position.x] == '\\')
                        {
                            if (ab.dir == direction::e) ab.dir = direction::south;
                            else if (ab.dir == direction::w) ab.dir = direction::n;
                            else if (ab.dir == direction::n) ab.dir = direction::w;
                            else if (ab.dir == direction::s) ab.dir = direction::e;
                            new_active_beams.push_back(ab);
                        }

                        if (map[ab.position.y][ab.position.x] == '-')
                        {
                            new_active_beams.push_back({ ab.position, direction::e });
                            new_active_beams.push_back({ ab.position, direction::w });
                        }

                        if (map[ab.position.y][ab.position.x] == '|')
                        {
                            new_active_beams.push_back({ ab.position, direction::n });
                            new_active_beams.push_back({ ab.position, direction::s });
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
        auto map = utility::io::transform_input_into_char_array(input_reader());
        set_star1_result(calculate_energy(map, { {-1,0}, direction::e}));

        min_max_counter<size_t> s2;
        for (long long y = 0; y < static_cast<long long>(map.size()); y++)
        {
            s2.check_value(calculate_energy(map, { {-1,y}, direction::e }));
            s2.check_value(calculate_energy(map, { {static_cast<point_2d::basic_type>(map[0].size()),y}, direction::w}));
        }

        for (long long x = 0; x < static_cast<long long>(map[0].size()); x++)
        {
            s2.check_value(calculate_energy(map, { {x,-1}, direction::s}));
            s2.check_value(calculate_energy(map, { {x, static_cast<point_2d::basic_type>(map.size())}, direction::n}));
        }
        set_star2_result(s2.maximum());
    }
};
