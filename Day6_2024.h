#pragma once
#include "helper.h"
using namespace utility::io;

template<>
class day<6, 2024> : public day_base<6,2024>
{
    const std::map<direction_2d::name, direction_2d::name> rotation =
    {
        { direction_2d::up, direction_2d::right },
        { direction_2d::right, direction_2d::down },
        { direction_2d::down, direction_2d::left },
        { direction_2d::left, direction_2d::up }
    };
    
    inline bool is_test() const override { return false; }

    std::set<point_2d_generic<long>> calculate_visited(const std::vector<std::string>& space, point_2d_generic<long> position, const char wall_id)
    {
        direction_2d::name direction = direction_2d::up;
        std::set<point_2d_generic<long>> visited;

        bool finish = false;
        while (!finish)
        {
            visited.insert(position);
            position = add(position, direction);
            if (is_outside_2D_array(space, position))
            {
                finish = true;
            }
            else if (get_value(space, position) == wall_id)
            {
                position = sub(position, direction);
                direction = rotation.at(direction);
            }
        }
        return visited;
    }

    bool is_loop(const std::vector<std::string>& space, point_2d_generic<long> position, const point_2d_generic<long> additional_obstacle, const char wall_id)
    {
        direction_2d::name dir = direction_2d::up;
        std::set<std::pair<point_2d_generic<long>, direction_2d::name> > visited;
        
        bool finish = false;
        while (!finish)
        {
            if (visited.contains({ position,dir })) return true;
            
            visited.insert({ position,dir });
            position = add(position, dir);

            if (is_outside_2D_array(space, position))
            {
                finish = true;
            }
            else if ((get_value(space, position) == wall_id) || (position == additional_obstacle))
            {
                position = sub(position, dir);
                dir = rotation.at(dir);
            }
        }
        return false;
    }

    size_t find_all_positions(const std::vector<std::string>& space, point_2d_generic<long> start_position, const std::set<point_2d_generic<long>>& additionals, const char wall_id)
    {
        size_t sum = 0;
        for (const auto p : additionals)
        {
            if (is_loop(space, start_position, p, '#')) sum++;
        }
        return sum;
    }

    void run_internal() override
    {
        std::vector<std::string> space;
        point_2d_generic<long> start_position;

        input_reader().read_by_line_until_condition_met_or_eof<size_t>(file_reader::read_condition::empty_line, [&](const auto& line, const auto)
            {
                if (line.find('^') != std::string::npos) start_position = { static_cast<long>(line.find('^')), static_cast<long>(space.size()) };
                space.push_back(line);
            });
 
        auto visited = calculate_visited(space, start_position, '#');
        set_star1_result(visited.size());

        visited.erase(start_position);
        set_star2_result(find_all_positions(space, start_position, visited, '#'));
    }
};