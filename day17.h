#pragma once
#include "helper.h"

class day17 : public day_base<17>
{
    int64_t get_velocity(int64_t x, int64_t dist)
    {
        int64_t x_dist = dist - x;
        double d = sqrt(x_dist * 8 + 1);
        return static_cast<int64_t>((1 + d) / 2);
    }


    bool is_target_hit(int64_t x, int64_t y, int64_t vx, int64_t vy, std::pair< int64_t, int64_t> x_limits, std::pair< int64_t, int64_t> y_limits)
    {
        int64_t n_x = x;
        int64_t n_y = y;

        while (true)
        {
            n_x = n_x + vx;
            n_y = n_y + vy;

            if (vx < 0) vx += 1;
            if (vx > 0) vx -= 1;
            vy -= 1;
            
            if ((n_x <= x_limits.second) && (n_x >= x_limits.first) && (n_y <= y_limits.second) && (n_y >= y_limits.first)) return true;

            if ((n_y < y) && (n_y < y_limits.first)) return false;

            if ((vx == 0) && ((n_x > x_limits.second) || (n_x < x_limits.first))) return false;

        }
        return false;
    }

    void run_interal() override
    {
        std::pair< int64_t, int64_t> x_limits = { 217, 240};
        std::pair< int64_t, int64_t> y_limits = {-126, -69};

        int64_t max_y_velocity = abs(y_limits.first) - 1;
        set_star1_result(max_y_velocity * (max_y_velocity + 1) / 2);

        auto x_min_velocity = get_velocity(0, x_limits.first);
        size_t hit_count = 0;
        size_t non_hit_count = 0;

        for (auto x_velocity = x_min_velocity; x_velocity <= x_limits.second; x_velocity++)
        {
            for (auto y_velocity = y_limits.first; y_velocity <= max_y_velocity; y_velocity++)
            {
                if (is_target_hit(0, 0, x_velocity, y_velocity, x_limits, y_limits))
                {
                    hit_count++;
                }
            }
        }

        set_star2_result(hit_count);
    }
};
