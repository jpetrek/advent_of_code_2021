#pragma once
#include "helper.h"

struct coords
{
    double x;
    double y;
    double z;
};

bool operator < (const coords& p1, const coords& p2)
{
    if (p1.x != p2.x)
    {
        return p1.x < p2.x;
    }
    if (p1.y != p2.y)
    {
        return p1.y < p2.y;
    }
    return p1.z < p2.z;
}

template<>
class day<18, 2022> : public day_base<18, 2022>
{
    const std::vector<coords> offsets = { {0, 0, 0.5}, {0, 0.5, 0}, {0.5, 0, 0}, {0, 0, -0.5}, {0, -0.5, 0}, {-0.5, 0, 0} };

    std::map<coords, size_t> convert_to_faces(const std::set<coords>& cubes)
    {
        std::map<coords, size_t> ret;
        for (const auto& c : cubes)
        {
            for (const auto& o : offsets)
            {
                helper::modify_value_in_map_safe<coords, size_t>(ret, { c.x + o.x, c.y + o.y, c.z + o.z }, 0, [&](auto& v) {return v++; });
            }
        }
        return ret;
    }

    void run_internal() override
    {
        std::set<coords> cubes;
        min_max_counter<double> x_limits;
        min_max_counter<double> y_limits;
        min_max_counter<double> z_limits;
        while (!input_reader().is_end_of_file())
        {
            auto [x, y, z] = helper::vector_to_tuple<3, double>(helper::split_and_convert<double>(input_reader().get_line(), ','));
            
            cubes.insert({ x,y,z });
            x_limits.check_value(x);
            y_limits.check_value(y);
            z_limits.check_value(z);
        }

        auto faces = convert_to_faces(cubes);
        set_star1_result(std::accumulate(std::begin(faces), std::end(faces), size_t(0), [](size_t r, const auto& v) {return v.second == 1 ? r + 1 : r; }));
        
        x_limits.check_value(x_limits.maximum() + 1);
        x_limits.check_value(x_limits.minimum() - 1);
        y_limits.check_value(y_limits.maximum() + 1);
        y_limits.check_value(y_limits.minimum() - 1);
        z_limits.check_value(z_limits.maximum() + 1);
        z_limits.check_value(z_limits.minimum() - 1);

        std::queue<coords> q;
        q.push({ x_limits.minimum(), y_limits.minimum(), z_limits.minimum() });

        std::set<coords> air;
        air.insert({ x_limits.minimum(), y_limits.minimum(), z_limits.minimum() });

        while (!q.empty())
        {
            coords actual = q.front(); q.pop();
            for (const auto& o : offsets)
            {
                coords move_by_one_queue = { actual.x + o.x * 2, actual.y + o.y * 2, actual.z + o.z * 2 };
                if ((x_limits.minimum() <= move_by_one_queue.x) && (move_by_one_queue.x <= x_limits.maximum()) &&
                    (y_limits.minimum() <= move_by_one_queue.y) && (move_by_one_queue.y <= y_limits.maximum()) &&
                    (z_limits.minimum() <= move_by_one_queue.z) && (move_by_one_queue.z <= z_limits.maximum()) &&
                    (!cubes.contains(move_by_one_queue)) &&
                    (!air.contains(move_by_one_queue)))
                {
                    air.insert(move_by_one_queue);
                    q.push(move_by_one_queue);
                }
            }
        }

        auto free_faces = convert_to_faces(air);
        set_star2_result(std::accumulate(std::begin(faces), std::end(faces), size_t(0), [&](size_t r, const auto& v) {return free_faces.contains(v.first) ? r + 1 : r; }));
    }
};

