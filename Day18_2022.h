#pragma once
#include "helper.h"

using namespace utility::arrays;
using namespace utility::geometry;
using namespace utility::math;
using namespace utility::string;

typedef point_3d_generic<double> coords;

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
                modify_value_in_map_safe<coords, size_t>(ret, { c.x + o.x, c.y + o.y, c.z + o.z }, 0, [&](const auto& v) {return v+1; });
            }
        }
        return ret;
    }

    bool is_value_in_range(coords value, const min_max_counter<double>& x_limits, const min_max_counter<double>& y_limits, const min_max_counter<double>& z_limits)
    {
        return  ((x_limits.minimum() <= value.x) && (value.x <= x_limits.maximum()) &&
            (y_limits.minimum() <= value.y) && (value.y <= y_limits.maximum()) &&
            (z_limits.minimum() <= value.z) && (value.z <= z_limits.maximum()));
    }

    void run_internal() override
    {
        std::set<coords> cubes;
        min_max_counter<double> x_limits;
        min_max_counter<double> y_limits;
        min_max_counter<double> z_limits;
        while (!input_reader().is_end_of_file())
        {
            auto [x, y, z] = vector_to_tuple<3, double>(split_and_convert<double>(input_reader().get_line(), ','));
            
            cubes.insert({ x,y,z });
            x_limits.check_value(x);
            y_limits.check_value(y);
            z_limits.check_value(z);
        }

        auto faces = convert_to_faces(cubes);
        set_star1_result(std::accumulate(std::begin(faces), std::end(faces), size_t(0), [](size_t r, const auto& v) {return v.second == 1 ? r + 1 : r; }));
        
        x_limits = modify_limits_symmetrically<double>(x_limits, 1);
        y_limits = modify_limits_symmetrically<double>(y_limits, 1);
        z_limits = modify_limits_symmetrically<double>(z_limits, 1);

        std::queue<coords> air_spread;
        air_spread.push({ x_limits.minimum(), y_limits.minimum(), z_limits.minimum() });

        std::set<coords> air;
        air.insert({ x_limits.minimum(), y_limits.minimum(), z_limits.minimum() });

        while (!air_spread.empty())
        {
            coords actual = air_spread.front(); air_spread.pop();
            for (const auto& o : offsets)
            {
                coords move_by_one_cube = { actual.x + o.x * 2, actual.y + o.y * 2, actual.z + o.z * 2 };
                if (is_value_in_range(move_by_one_cube, x_limits, y_limits, z_limits) && !cubes.contains(move_by_one_cube) && !air.contains(move_by_one_cube))
                {
                    air.insert(move_by_one_cube);
                    air_spread.push(move_by_one_cube);
                }
            }
        }

        auto free_faces = convert_to_faces(air);
        set_star2_result(std::accumulate(std::begin(faces), std::end(faces), size_t(0), [&free_faces](size_t r, const auto& v) {return free_faces.contains(v.first) ? r + 1 : r; }));
    }
};

