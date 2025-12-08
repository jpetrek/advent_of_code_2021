#pragma once
#include "helper.h"
using namespace utility::io;

typedef point_2d_generic<int64_t> position;

template<>
class day<7, 2025> : public day_base<7,2025>
{
    inline bool is_test() const override { return false; }
    void run_internal() override
    {
        uint64_t sum_1 = 0;
        std::set<position> splitters;
        position start;
        int64_t height = 0;

        input_reader().read_by_line_until_condition_met_or_eof<size_t>(file_reader::no_condition(), [&](const auto& line, const auto line_number)
            {
                for (int64_t x = 0; x < static_cast<int64_t>(line.size()); x++)
                {
                    if (line[x] == '^') splitters.insert({ x,height });
                    if (line[x] == 'S') start = { x,height };
                }
                height = line_number;
            });

        std::map<position, size_t> rays = { { start,1 } };

        while (true)
        {
            std::map<position, size_t> new_rays;
            for (const auto& ray : rays)
            {
                if (ray.first.y + 1 < height)
                {
                    if (splitters.contains({ ray.first.x, ray.first.y + 1 }))
                    {
                        sum_1++;
                        modify_map(new_rays, { ray.first.x - 1, ray.first.y + 1 }, 0, [&](auto& v) { v += ray.second; });
                        modify_map(new_rays, { ray.first.x + 1, ray.first.y + 1 }, 0, [&](auto& v) { v += ray.second; });
                    }
                    else
                    {
                        modify_map(new_rays, { ray.first.x, ray.first.y + 1 }, 0, [&](auto& v) { v += ray.second; });
                    }
                }
            }

            if (new_rays.size() == 0) break;
            rays = new_rays;
        }
        
        set_star1_result(sum_1);
        set_star2_result(accumulate_generic<int64_t>(rays, 0, [](const auto v, const auto& r) { return v + r.second; }));
    }
};