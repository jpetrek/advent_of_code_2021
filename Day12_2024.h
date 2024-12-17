#pragma once
#include "helper.h"
using namespace utility::arrays;
using namespace utility::string;
using namespace utility::math;


template<>
class day<12, 2024> : public day_base<12, 2024>
{
    typedef std::vector<std::vector<char>> garden_map;
    typedef point_2d_generic<long> coords;
    typedef direction_2d_generic<long>::name dir;

    const std::vector<direction::name> directions = { direction::up, direction::right, direction::down, direction::left };

    inline bool is_test() const override { return false; }

    void identify_garden(const garden_map& space, coords actual, std::set<coords>& garden, std::set<std::pair<coords, dir>>& borders, char garden_id)
    {
        for (auto direction : directions)
        {
            coords new_position = add(actual, direction);
            if (!garden.contains(new_position))
            {
                if (is_outside_2D_array(space, new_position) || get_value(space, new_position) != garden_id)
                {
                    borders.insert({ new_position, direction });
                }
                else
                {
                    garden.insert(new_position);
                    identify_garden(space, new_position, garden, borders, garden_id);
                }
            }
        }
    }

    size_t number_of_borders(direction::name direction, const std::set<std::pair<coords, dir>>& borders)
    {
        std::map<long, std::vector<long>> border_position_map;
        for (auto border : borders)
        {
            if (border.second == direction)
            {
                if ((direction == direction::name::up) || (direction == direction::name::down))
                {
                    modify_map(border_position_map, border.first.y, {}, [&](auto& t) {t.push_back(border.first.x); });
                }
                else
                {
                    modify_map(border_position_map, border.first.x, {}, [&](auto& t) {t.push_back(border.first.y); });
                }
            }
        }

        size_t summary = 0;
        for (auto& line : border_position_map)
        {
            std::sort(std::begin(line.second), std::end(line.second));
            if (line.second.size() > 0)
            {
                size_t count = 1;
                if (line.second.size() > 1)
                {
                    for (size_t i = 1; i < line.second.size(); i++)
                    {
                        if (abs(line.second[i - 1] - line.second[i]) > 1) count++;
                    }
                }
                summary += count;
            }
        }
        return summary;
    }


    void run_internal() override
    {
        garden_map space = utility::io::transform_input_into_char_array(input_reader());
        std::set<coords>  identified;
        size_t sum1 = 0;
        size_t sum2 = 0;
        foreach_in_2d_array<long>(space, [&](const auto& position, auto v)
            {
                if (!identified.contains(position))
                {
                    std::set<coords> garden = { position };
                    std::set<std::pair<coords, dir>> borders;
                    identify_garden(space, position, garden, borders, get_value(space, position));

                    sum1 += garden.size() * borders.size();
                    sum2 += (number_of_borders(direction::name::up, borders) + number_of_borders(direction::name::down, borders) + number_of_borders(direction::name::right, borders) + number_of_borders(direction::name::left, borders)) * garden.size();

                    identified.merge(garden);
                }
            });
        set_star1_result(sum1);
        set_star2_result(sum2);
    }
};