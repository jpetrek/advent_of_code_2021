    #pragma once
#include "helper.h"

using namespace utility::io;

template<>
class day<9, 2025> : public day_base<9,2025>
{
    typedef point_2d_generic<int64_t> point;
    typedef interval_generic< int64_t> interval;

    auto row_intervals_at_y(const std::vector<point>& poly, const int64_t y0)
    {
        std::vector<interval> ivals;
        std::vector<int64_t> xints;

        size_t n = poly.size();
        for (size_t i = 0; i < n; ++i) 
        {
            point a = poly[i];
            point b = poly[(i + 1) % n];
            if (a.y == b.y && a.y == y0) 
            {
                ivals.push_back({ std::min(a.x, b.x), std::max(a.x, b.x) });
                continue;
            }

            if (a.x == b.x) 
            {
                int64_t ymin = std::min(a.y, b.y);
                int64_t ymax = std::max(a.y, b.y);
                if (ymin <= y0 && y0 < ymax) 
                {
                    xints.push_back(a.x);
                }
            }
        }

        sort(xints.begin(), xints.end());
        for (size_t i = 0; i + 1 < xints.size(); i += 2) 
        {
            ivals.push_back({ xints[i], xints[i + 1] });
        }

        return merge_intervals(ivals);
    }

    auto build_all_row_intervals(const std::vector<point>& poly)
    {
        std::map<int64_t, std::vector<interval>> out;
        int64_t minY = poly[0].y, maxY = poly[0].y;
        for (auto& p : poly) 
        {
            minY = std::min(minY, p.y);
            maxY = std::max(maxY, p.y);
        }

        for (int64_t y = minY; y <= maxY; ++y)
        {
            auto row = row_intervals_at_y(poly, y);
            if (!row.empty()) out[y] = row;
        }
        return out;
    }

    bool is_row_covered_by_interval(const std::vector<interval>& ivals, int64_t x1, int64_t x2)
    {
        for (const auto& iv : ivals) 
        {
            if (iv.min <= x1 && iv.max >= x2) return true;
            if (iv.min > x1) break;
        }
        return false;
    }

    bool is_rectangle_inside(int64_t x1, int64_t y1, int64_t x2, int64_t y2, const std::map<int64_t, std::vector<interval>>& intervals_at_y)
    {
        if (x1 > x2) std::swap(x1, x2);
        if (y1 > y2) std::swap(y1, y2);
        for (int64_t y = y1; y <= y2; ++y)
        {
            auto it = intervals_at_y.find(y);
            if (it == intervals_at_y.end()) return false;
            if (!is_row_covered_by_interval(it->second, x1, x2)) return false;
        }
        return true;
    }

    inline bool is_test() const override { return false; }
   
    void run_internal() override
    {
        std::vector<point> points;
        input_reader().read_by_line_until_condition_met_or_eof<size_t>(file_reader::no_condition(), [&](const auto& line, const auto)
            {
                auto coords = split_and_convert< int64_t>(line, ',', stoll());
                points.push_back({ coords[0], coords[1] });
        });

        auto row_intervals = build_all_row_intervals(points);
        std::mutex m;
        min_max_counter<uint64_t> s1;
        min_max_counter<uint64_t> s2;

        size_t pi = 0;
        std::vector<std::pair<point, point>> combinations;
        for (const auto& p1 : points)
        {
            for (const auto& p2 : points)
            {
                if (p1 != p2)
                {
                    combinations.push_back({ p1,p2 });
                }
            }
        }

        std::for_each( std::execution::par, std::begin(combinations), std::end(combinations),[&](auto&& item)
        {
                auto p1 = item.first;
                auto p2 = item.second;
                auto dx = 1 + (p1.x > p2.x ? p1.x - p2.x : p2.x - p1.x);
                auto dy = 1 + (p1.y > p2.y ? p1.y - p2.y : p2.y - p1.y);
                {
                    std::lock_guard<std::mutex> lock(m);
                    s1.check_value(dx * dy);
                }
                if (is_rectangle_inside(p1.x, p1.y, p2.x, p2.y, row_intervals))
                {
                    std::lock_guard<std::mutex> lock(m);
                    s2.check_value(dx * dy);
                }
        });

        set_star1_result(s1.maximum());
        set_star2_result(s2.maximum());
    }   
};