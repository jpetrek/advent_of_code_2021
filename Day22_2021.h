#pragma once
#include "helper.h"

template<>
class day<22, 2021> : public day_base<22, 2021>
{
    std::pair<size_t, size_t> get_range(const std::vector<int64_t>& ranges, int64_t value_min, int64_t value_max)
    {
        if (value_min > value_max) throw;
        int64_t i0 = 0;
        int64_t i1 = 0;
        for (size_t i = 0; i < ranges.size(); i++)
        {
            if (ranges[i] == value_min)
            {
                i0 = i;
            }
            if (ranges[i] == value_max)
            {
                i1 = i;
            }
        }
        return { i0,i1 };
    }

    void run_internal() override
    {
        struct input_record
        {
            int64_t x_min, x_max;
            int64_t y_min, y_max;
            int64_t z_min, z_max;
            bool on_off;
        };

        std::map<point_3d, bool> cache;
        std::vector<input_record> data;

        while (!input_reader().is_end_of_file())
        {
            auto line = input_reader().get_line();
            if (line.find("on") != std::string::npos)
            {
                input_record ir;
                ir.on_off = true;
                auto r = sscanf_s(line.c_str(), "on x = %I64d..%I64d, y = %I64d..%I64d, z = %I64d..%I64d", &ir.x_min, &ir.x_max, &ir.y_min, &ir.y_max, &ir.z_min, &ir.z_max);
                if (r != 6) throw;
                data.push_back(ir);
            }
            else
            {
                input_record ir;
                ir.on_off = false;
                auto r = sscanf_s(line.c_str(), "off x = %I64d..%I64d, y = %I64d..%I64d, z = %I64d..%I64d", &ir.x_min, &ir.x_max, &ir.y_min, &ir.y_max, &ir.z_min, &ir.z_max);
                if (r != 6) throw;
                data.push_back(ir);
            }
        }

        for (const auto& i : data)
        {
            if ((i.x_min >= -50) && (i.x_max <= 50) &&
                (i.y_min >= -50) && (i.y_max <= 50) &&
                (i.z_min >= -50) && (i.z_max <= 50))
            {
                for (int64_t x = i.x_min; x <= i.x_max; x++)
                    for (int64_t y = i.y_min; y <= i.y_max; y++)
                        for (int64_t z = i.z_min; z <= i.z_max; z++)
                        {
                            cache[{x, y, z}] = i.on_off;
                        }
            }
        }

        size_t sum = 0;
        for (const auto& c : cache)
        {
            if (c.second) sum++;
        }
        
        set_star1_result(sum);

        std::set<int64_t> x_axis;
        std::set<int64_t> y_axis;
        std::set<int64_t> z_axis;

        for (const auto& i : data)
        {
            x_axis.insert(i.x_min);
            x_axis.insert(i.x_max+1);
            y_axis.insert(i.y_min);
            y_axis.insert(i.y_max+1);
            z_axis.insert(i.z_min);
            z_axis.insert(i.z_max+1);
        }
        
        std::vector<int64_t> ranges_x(x_axis.begin(), x_axis.end());
        std::vector<int64_t> ranges_y(y_axis.begin(), y_axis.end());
        std::vector<int64_t> ranges_z(z_axis.begin(), z_axis.end());

        std::vector<std::vector<std::vector<bool>>> range_grid = std::vector<std::vector<std::vector<bool>>>(ranges_x.size()-1, std::vector<std::vector<bool>>(ranges_y.size()-1, std::vector<bool>(ranges_z.size()-1, false)));

        for (const auto& i : data)
        {
            auto x_values = get_range(ranges_x, i.x_min, i.x_max+1);
            for (auto x = x_values.first; x < x_values.second; x++)
            {
                auto y_values = get_range(ranges_y, i.y_min, i.y_max+1);
                for (auto y = y_values.first; y < y_values.second; y++)
                {
                    auto z_values = get_range(ranges_z, i.z_min, i.z_max+1);
                    for (auto z = z_values.first; z < z_values.second; z++)
                    {
                        range_grid[x][y][z] = i.on_off;
                    }
                }
            }
        }

        sum = 0;
        for (size_t x = 0; x < range_grid.size(); x++)
            for (size_t y = 0; y < range_grid[x].size(); y++)
                for (size_t z = 0; z < range_grid[x][y].size(); z++)
                {
                    if (range_grid[x][y][z])
                    {
                        sum += (ranges_x[x + 1] - ranges_x[x]) * (ranges_y[y + 1] - ranges_y[y]) * (ranges_z[z + 1] - ranges_z[z]);
                    }
                }

        set_star2_result(sum);
    }
};
