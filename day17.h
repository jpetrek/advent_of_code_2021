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
        //std::pair< int64_t, int64_t> x_limits = { 254, 275 };
        //std::pair< int64_t, int64_t> y_limits = { -107, -57 };
        // Simplified calculation of max height doesn't work here!
        // Simply because there is not any x-velocity to be zero within the area
        // x_velocity = 23 zeroes at 253, x_velocity 24 zeroes at 277
        // if such x_velocity exists then max_h =  max_y_velocity * (max_y_velocity -1) / 2

        std::pair< int64_t, int64_t> x_limits = { 217, 240};
        std::pair< int64_t, int64_t> y_limits = {-126, -69};

        int64_t max_y_velocity = abs(y_limits.first) - 1;
        auto x_min_velocity = get_velocity(0, x_limits.first);

        size_t hit_count = 0;
        size_t max_h = 0;
        std::pair<int64_t, int64_t> max_h_velocities;

        for (auto x_velocity = x_min_velocity - 1; x_velocity <= x_limits.second; x_velocity++)
        {
            for (auto y_velocity = y_limits.first; y_velocity <= max_y_velocity; y_velocity++)
            {
                if (is_target_hit(0, 0, x_velocity, y_velocity, x_limits, y_limits))
                {
                    if (y_velocity > 0)
                    {
                        size_t h = y_velocity * (y_velocity -1) / 2;
                        if (h > max_h)
                        {
                            max_h = h;
                            max_h_velocities = { x_velocity , y_velocity };
                        }
                    }
                    hit_count++;
                }
            }
        }

        std::cout << "Max height velocities: vx =" << max_h_velocities.first << ", vy=" << max_h_velocities.second << std::endl;
        set_star1_result(max_h);
        set_star2_result(hit_count);
    }
};
