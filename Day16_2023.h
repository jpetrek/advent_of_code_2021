#pragma once
#include "helper.h"

template<>
class day<16, 2023> : public day_base<16, 2023>
{
    struct beam
    {
        point_2d_generic<long long> position;
        direction_2D ::name dir;
    };

    size_t calculate_energy(const std::vector<std::vector<char>>& map, beam init_beam)
    {
        std::vector<std::vector<size_t>> energy(map.size(), std::vector<size_t>(map[0].size(), 0));
        std::map<std::pair<long long, long long>, direction_2D::name> direction_cache;
        
        std::vector<beam> active_beams;
        active_beams.push_back(init_beam);
        direction_cache[{init_beam.position.x, init_beam.position.y}] = init_beam.dir;
        
        while (active_beams.size() > 0)
        {
            std::vector<beam> new_active_beams;
            for (auto ab : active_beams)
            {
                if ((ab.position.x + direction_2D::get_diference(ab.dir).dx >= 0) && (ab.position.x + direction_2D::get_diference(ab.dir).dx < static_cast<long long>(map[0].size())) &&
                    (ab.position.y + direction_2D::get_diference(ab.dir).dy >= 0) && (ab.position.y + direction_2D::get_diference(ab.dir).dy < static_cast<long long>(map.size())))
                {
                    ab.position.x += direction_2D::get_diference(ab.dir).dx;
                    ab.position.y += direction_2D::get_diference(ab.dir).dy;

                    if (!direction_cache.contains({ ab.position.x, ab.position.y }) || direction_cache.at({ ab.position.x, ab.position.y }) != ab.dir)
                    {
                        direction_cache[{ ab.position.x, ab.position.y }] = ab.dir;
                        energy[ab.position.y][ab.position.x]++;

                        if ((map[ab.position.y][ab.position.x] == '.')||
                            (map[ab.position.y][ab.position.x] == '-' && ((ab.dir == direction_2D::east) || (ab.dir == direction_2D::west)))||
                            (map[ab.position.y][ab.position.x] == '|' && ((ab.dir == direction_2D::north) || (ab.dir == direction_2D::south))))
                        {
                            new_active_beams.push_back(ab);
                        }

                        if (map[ab.position.y][ab.position.x] == '/')
                        {
                            if (ab.dir == direction_2D::east) ab.dir = direction_2D::north;
                            else if (ab.dir == direction_2D::west) ab.dir = direction_2D::south;
                            else if (ab.dir == direction_2D::north) ab.dir = direction_2D::east;
                            else if (ab.dir == direction_2D::south) ab.dir = direction_2D::west;
                            new_active_beams.push_back(ab);
                        }

                        if (map[ab.position.y][ab.position.x] == '\\')
                        {
                            if (ab.dir == direction_2D::east) ab.dir = direction_2D::south;
                            else if (ab.dir == direction_2D::west) ab.dir = direction_2D::north;
                            else if (ab.dir == direction_2D::north) ab.dir = direction_2D::west;
                            else if (ab.dir == direction_2D::south) ab.dir = direction_2D::east;
                            new_active_beams.push_back(ab);
                        }

                        if (map[ab.position.y][ab.position.x] == '-')
                        {
                            new_active_beams.push_back({ ab.position, direction_2D::east });
                            new_active_beams.push_back({ ab.position, direction_2D::west });
                        }

                        if (map[ab.position.y][ab.position.x] == '|')
                        {
                            new_active_beams.push_back({ ab.position, direction_2D::north });
                            new_active_beams.push_back({ ab.position, direction_2D::south });
                        }
                    }
                }
            }
            active_beams = new_active_beams;
        }

        return helper::accumulate_generic<size_t>(energy, 0, [](size_t r, const auto& row) {return r + helper::accumulate_generic<size_t>(row, 0, [](size_t r, const auto& n) {return  n > 0 ? r + 1 : r; }); });
    }

    void run_internal() override
    {
        //set_is_test(true);
        std::vector<std::vector<char>> map = helper::transform_input_into_array<char>(input_reader(), [](const char c) {return c; });
        set_star1_result(calculate_energy(map, { {-1,0}, direction_2D::east }));

        min_max_counter<size_t> s2;
        for (long long y = 0; y < static_cast<long long>(map.size()); y++)
        {
            s2.check_value(calculate_energy(map, { {-1,y}, direction_2D::east }));
            s2.check_value(calculate_energy(map, { {static_cast<long long>(map[0].size()),y}, direction_2D::west}));
        }

        for (long long x = 0; x < static_cast<long long>(map[0].size()); x++)
        {
            s2.check_value(calculate_energy(map, { {x,-1}, direction_2D::south }));
            s2.check_value(calculate_energy(map, { {x, static_cast<long long>(map.size())}, direction_2D::west }));
        }
        set_star2_result(s2.value_maximum);
    }
};
