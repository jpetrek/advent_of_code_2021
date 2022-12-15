#pragma once
#include "helper.h"

template<>
class day<15, 2022> : public day_base<15, 2022>
{
    long get_manhatan_distance(std::pair<long, long> p1, std::pair<long, long> p2)
    {
        return abs(p1.first - p2.first) + abs(p1.second - p2.second);
    }

    struct sensor
    {
        std::pair<long, long> position;
        long distance;
    };

    long get_missing_x(long min_x, long max_x, std::vector<std::pair<long, long>> intervals)
    {
        for (auto& i : intervals)
        {
            if (i.first < min_x) i.first = min_x;
            if (i.second < min_x) i.second = min_x;
            if (i.second > max_x) i.second = max_x;
            if (i.first > max_x) i.first = max_x;
        }

        std::sort(std::begin(intervals), std::end(intervals), [](const auto& a, const auto& b) {return a.first < b.first; });

        long last_x = min_x;

        while (intervals.size() > 0)
        {
            auto current = intervals.front();
            intervals.erase(intervals.begin());
            
            for (auto& i : intervals)
            {
                if ((i.first >= current.first) && (i.first < current.second)) i.first = current.second;
                if ((i.second >= current.first) && (i.second < current.second)) i.second = current.second;
            }

            if (last_x != current.first)
            {
                return (current.first + last_x) / 2;
            }

            last_x = current.second;
        }

        return min_x - 1;
    }

    void run_interal() override
    {

        min_max_counter<long> x_limits;
        std::vector<sensor> data;
        while (!input_reader().is_end_of_file())
        {
            auto line = input_reader().get_line();
            std::pair<long, long> s,b;
            sscanf_s(line.data(), "Sensor at x = %d, y = %d: closest beacon is at x = %d, y = %d", &s.first, &s.second, &b.first, &b.second);
            data.push_back({s, get_manhatan_distance(s, b) });
            x_limits.check_value(s.first - data.back().distance);
            x_limits.check_value(s.first + data.back().distance);
        }

        //naive but working for star one
        size_t c = 0;
        long y = 2000000;
        for (long x = x_limits.minimum(); x <= x_limits.maximum(); x++)
        {
            bool no_beacon = false;
            for (size_t i = 0; i < data.size(); i++)
            {
                if (get_manhatan_distance(data[i].position, { x,y }) <= data[i].distance)
                {
                    no_beacon = true;
                    break;
                }
            }
            if (no_beacon) c++;
        }

        set_star1_result(c - 1);

        //here comes the tricky part, optimization needed
        long x_min = 0;
        long y_min = 0;
        long x_max = 4000000;
        long y_max = 4000000;
        for (long y = y_min; y <= y_max; y++)
        {
            std::vector<std::pair<long, long>> intervals;
            for (const auto& d : data)
            {
                long diff = abs(y - d.position.second);
                long distance_shrink = d.distance - diff;
                if (distance_shrink >= 0)
                {
                    intervals.push_back({ d.position.first - distance_shrink, d.position.first + distance_shrink });
                }
            }

            auto x = get_missing_x(x_min, x_max, intervals);
            
            if (x >= x_min)
            {
                set_star2_result<long long>(static_cast<long long>(x_max) * static_cast<long long>(x) + static_cast<long long>(y));
                break;
            }
        }
    }
};
