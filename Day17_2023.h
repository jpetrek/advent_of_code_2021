#pragma once
#include "helper.h"

template<>
class day<17, 2023> : public day_base<17, 2023>
{
    typedef point_2d_generic<size_t> point_2d;
    typedef std::tuple<point_2d, direction_2d::name, size_t> state_key;
    typedef std::tuple<state_key, size_t> state;
    typedef std::priority_queue < state, std::vector<state>, decltype([](const state& s1, const state& s2) { return get<1>(s1) > get<1>(s2); }) > state_queue_prioritized_by_lower_heat_loss;

    bool is_test() const override
    {
        return false;
    }

    size_t calculate_s1(const std::vector<std::vector<size_t>>& map, state_queue_prioritized_by_lower_heat_loss queue, const point_2d end_point)
    {
        std::map <state_key, size_t> cache;
        while (!queue.empty())
        {
            auto  [key, heat_loss] = queue.top(); queue.pop();
            auto& [pos, dir, dir_count] = key;

            heat_loss += map[pos.y][pos.x];

            if (cache.contains({ pos, dir, dir_count }) && heat_loss >= cache.at({ pos, dir,dir_count })) continue;

            cache[{ pos, dir, dir_count }] = heat_loss;

            if (pos == end_point)
            {
                return heat_loss;
            }

            if (dir == direction_2d::n)
            {
                if ((pos.y > 0) && (dir_count < 3)) queue.push({ {{ pos.x, pos.y - 1 }, direction_2d::n, dir_count + 1}, heat_loss });
                if (pos.x > 0) queue.push({ {{ pos.x - 1, pos.y }, direction_2d::w, 1}, heat_loss });
                if (pos.x < map[0].size() - 1) queue.push({ {{ pos.x + 1, pos.y }, direction_2d::e, 1},heat_loss });
            }
            else if (dir == direction_2d::s)
            {
                if ((pos.y < map.size() - 1) && (dir_count < 3)) queue.push({ { { pos.x, pos.y + 1 }, direction_2d::s, dir_count + 1}, heat_loss });
                if (pos.x > 0) queue.push({ { { pos.x - 1, pos.y }, direction_2d::w, 1 }, heat_loss });
                if (pos.x < map[0].size() - 1) queue.push({ {{ pos.x + 1, pos.y }, direction_2d::e, 1}, heat_loss });
            }
            else if (dir == direction_2d::w)
            {
                if ((pos.x > 0) && (dir_count < 3)) queue.push({ { { pos.x - 1, pos.y }, direction_2d::w, dir_count + 1}, heat_loss });
                if (pos.y > 0) queue.push({ { { pos.x, pos.y - 1 }, direction_2d::n, 1}, heat_loss });
                if (pos.y < map.size() - 1) queue.push({ { { pos.x, pos.y + 1 }, direction_2d::s, 1}, heat_loss });
            }
            else if (dir == direction_2d::e)
            {
                if ((pos.x < map[0].size() - 1) && (dir_count < 3)) queue.push({ {{ pos.x + 1, pos.y }, direction_2d::e, dir_count + 1}, heat_loss });
                if (pos.y > 0) queue.push({ { { pos.x, pos.y - 1 }, direction_2d::n, 1}, heat_loss });
                if (pos.y < map.size() - 1) queue.push({ { { pos.x, pos.y + 1 }, direction_2d::s, 1}, heat_loss });
            }
        }
        return 0;
    }

    size_t calculate_s2(const std::vector<std::vector<size_t>>& map, state_queue_prioritized_by_lower_heat_loss queue, const point_2d end_point)
    {
        std::map <state_key, size_t> cache;
        while (!queue.empty())
        {
            auto  [key, heat_loss] = queue.top(); queue.pop();
            auto& [pos, dir, dir_count] = key;

            heat_loss += map[pos.y][pos.x];

            if (cache.contains({ pos, dir, dir_count }) && heat_loss >= cache.at({ pos, dir,dir_count })) continue;

            cache[{ pos, dir, dir_count }] = heat_loss;

            if (pos == end_point)
            {
                return heat_loss;
            }

            if (dir == direction_2d::n)
            {
                if ((dir_count < 10) && (pos.y > 0)) queue.push({ { { pos.x, pos.y - 1 }, direction_2d::n, dir_count + 1 }, heat_loss });
                if ((dir_count > 3) && (pos.x > 0)) queue.push({ { { pos.x - 1, pos.y }, direction_2d::w, 1 }, heat_loss });
                if ((dir_count > 3) && (pos.x < map[0].size() - 1)) queue.push({ { { pos.x + 1, pos.y }, direction_2d::e, 1 },heat_loss });
            }
            else if (dir == direction_2d::s)
            {
                if ((dir_count < 10) && (pos.y < map.size() - 1)) queue.push({ { { pos.x, pos.y + 1 }, direction_2d::s, dir_count + 1 }, heat_loss });
                if ((dir_count > 3) && (pos.x > 0)) queue.push({ { { pos.x - 1, pos.y }, direction_2d::w, 1 }, heat_loss });
                if ((dir_count > 3) && (pos.x < map[0].size() - 1)) queue.push({ { { pos.x + 1, pos.y }, direction_2d::e, 1 }, heat_loss });
            }
            else if (dir == direction_2d::w)
            {
                if ((dir_count < 10) && (pos.x > 0)) queue.push({ { { pos.x - 1, pos.y }, direction_2d::w, dir_count + 1 }, heat_loss });
                if ((dir_count > 3) && (pos.y > 0)) queue.push({ { { pos.x, pos.y - 1 }, direction_2d::n, 1 }, heat_loss });
                if ((dir_count > 3) && (pos.y < map.size() - 1)) queue.push({ { { pos.x, pos.y + 1 }, direction_2d::s, 1 }, heat_loss });
            }
            else if (dir == direction_2d::e)
            {
                if ((dir_count < 10) && (pos.x < map[0].size() - 1)) queue.push({ { { pos.x + 1, pos.y }, direction_2d::e, dir_count + 1 }, heat_loss });
                if ((dir_count > 3) && (pos.y > 0)) queue.push({ { { pos.x, pos.y - 1 }, direction_2d::n, 1 }, heat_loss });
                if ((dir_count > 3) && (pos.y < map.size() - 1)) queue.push({ { { pos.x, pos.y + 1 }, direction_2d::s, 1 }, heat_loss });
            }
        }
        return 0;

    }

    void run_internal() override
    {
        auto m = utility::io::transform_input_into_array<size_t>(input_reader(), [](const char c) {return c - '0'; });

        point_2d end_point = { m[0].size() - 1, m.size() - 1 };

        state_queue_prioritized_by_lower_heat_loss queue;
        queue.push({ {{ 1,0 }, direction_2d::e, 1},0 });
        queue.push({ {{ 0,1 }, direction_2d::s, 1},0 });

        set_star1_result(calculate_s1(m, queue, end_point));
        set_star2_result(calculate_s2(m, queue, end_point));
    }
};


