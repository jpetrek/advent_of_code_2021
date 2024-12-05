#pragma once
#include "helper.h"

typedef std::set<point_2d_generic<size_t>> map_points;


template<>
class day<14, 2023> : public day_base<14, 2023>
{
    std::pair< map_points, map_points > do_tilt(const map_points& solid_rocks, map_points empty_space, map_points movable_rocks, const direction_2d::diference direction, const size_t width, const size_t height)
    {
        bool move = true;
        while (move)
        {
            move = false;
            map_points new_movable_rocks;
            for (const auto& mr : movable_rocks)
            {
                size_t nx = mr.x;
                size_t ny = mr.y;
                while ((nx + direction.dx >= 0) && (nx + direction.dx < width) && (ny + direction.dy >= 0) && (ny + direction.dy < height) && empty_space.contains({ nx + direction.dx,ny + direction.dy }))
                {
                    nx += direction.dx;
                    ny += direction.dy;
                }
                if ((nx != mr.x) || (ny != mr.y))
                {
                    move = true;
                    new_movable_rocks.insert({ nx,ny });
                    empty_space.insert({ mr.x, mr.y });
                    empty_space.erase({ nx,ny });
                }
                else
                {
                    new_movable_rocks.insert({ nx,ny });
                }
            }
            movable_rocks = new_movable_rocks;
        }
        return { empty_space , movable_rocks };
    }

    size_t calculate_load(const map_points& movable_rocks, size_t width, size_t height)
    {
        size_t sum = 0;
        for (size_t y = 0; y < height; y++)
        {
            for (size_t x = 0; x < width; x++)
            {
                if (movable_rocks.contains({ x,y })) sum += height - y;
            }
        }
        return sum;
    }

    void run_internal() override
    {
        map_points movable_rocks;
        map_points empty_space;
        map_points solid_rocks;
        size_t w = 0;
        size_t h = 0;
        size_t y = 0;
        while (!input_reader().is_end_of_file())
        {
            auto line = input_reader().get_line();
            w = line.size();
            for (size_t x = 0; x < w; x++)
            {
                if (line[x] == 'O') movable_rocks.insert({ x,y });
                if (line[x] == '#') solid_rocks.insert({ x,y });
                if (line[x] == '.') empty_space.insert({ x,y });
            }
            y++;
        }
        h = y;
        
        auto [es, mr]= do_tilt(solid_rocks, empty_space, movable_rocks, direction_2d::name_to_diff(direction_2d::name::north), w, h);
        set_star1_result(calculate_load(mr, w, h));

        std::map<map_points, size_t> cache;
        std::map<size_t, size_t> results;
        size_t cycle = 0;
        bool cycle_identified = false;

        while (!cycle_identified)
        {
            cycle++;
            for (const auto direction : direction_2d::generate_diferences({ direction_2d::name::north, direction_2d::name::west, direction_2d::name::south, direction_2d::name::east }))
            {
                auto [es, mr] = do_tilt(solid_rocks, empty_space, movable_rocks, direction, w, h);
                empty_space = es;
                movable_rocks = mr;
            }

            if (cache.contains(movable_rocks))
            {
                cycle_identified = true;
                auto cycle_start = cache.at(movable_rocks);
                auto cycle_length = cycle - cache.at(movable_rocks);
                set_star2_result(results.at(cycle_start + (1000000000 - cycle_start) % cycle_length));
            }
            else
            {
                cache[movable_rocks] = cycle;
                results[cycle] = calculate_load(movable_rocks, w, h);
            }
        }
    }
};
