#pragma once
#include "helper.h"

template<>
class day<21, 2023> : public day_base<21, 2023>
{   
    typedef point_2d_generic<int64_t> point_2d;
    enum space
    {
        start = 0,
        rock = 1,
        gplot = 2
    };

    double extrapolate_quadratic(size_t a0, size_t a1, size_t a2, double n)
    {
        double b0 = static_cast<double>(a0);
        double b1 = static_cast<double>(a1 - a0);
        double b2 = static_cast<double>(a2 - a1);
        double c = b0 + b1 * n + std::floor((n * (n - 1) / 2.0)) * (b2 - b1);
        return c;
    }

    size_t get_number(const size_t limit, const std::map<point_2d, space>& field, const size_t w, const size_t h, const point_2d start)
    {
        size_t reach = 0;
        size_t hit = 0;
        std::queue<std::pair<point_2d, size_t>> q;
        std::map<point_2d, size_t> cache;
        q.push({ start, 0 });
        while (!q.empty())
        {
            auto item = q.front(); q.pop();
            if ((!cache.contains(item.first)) || cache.at(item.first) != item.second)
            {
                if (item.second <= limit) cache[item.first] = item.second;
                if (item.second < limit)
                {
                    std::vector<std::pair<int, int>> directions = { {1,0}, {0,1}, {-1,0}, {0,-1} };
                    for (auto d : directions)
                    {
                        int64_t nx = item.first.x + d.first;
                        int64_t ny = item.first.y + d.second;
                        int64_t rx = nx < 0 ? (w - ((-nx) % w)) % w : nx % w;
                        int64_t ry = ny < 0 ? (h - ((-ny) % h)) % h : ny % h;
                        if (field.at({ rx,ry }) == space::gplot)
                        {
                            q.push({ { nx,ny }, item.second + 1 });
                        }
                    }
                }
                else if (item.second == limit)
                {
                    reach++;
                }
            }
        }
        return reach;
    }

    bool is_test() const override
    {
        return false;
    }

    void run_internal() override
    {
        std::map<point_2d, space> field;
        int64_t w = 0;
        int64_t h = 0;
        point_2d start = { 0,0 };

        while (!input_reader().is_end_of_file())
        {
            auto l = input_reader().get_line();
            if (l.size() > 0)
            {
                w = l.size();
                for (int64_t i =0; i<static_cast<int64_t>(l.size()); i++)
                {
                    if (l[i] == '.') field[{i, h}] = space::gplot;
                    else if (l[i] == 'S') 
                    {
                        start = { i, h };
                        field[{i, h}] = space::gplot;
                    }
                    else if (l[i] == '#') field[{i, h}] = space::rock;
                }
                h++;
            }
        }

        set_star1_result(get_number(64, field, w, h, start));
        set_star2_result(static_cast<size_t>(extrapolate_quadratic(get_number(65, field, w, h, start), get_number(65+131, field, w, h, start), get_number(65 + 131 + 131, field, w, h, start), 202300)));
    }
};

