#pragma once
#include "helper.h"

template<>
class day<19, 2021> : public day_base<19, 2021>
{

    point_3d transform_x(size_t a, const point_3d i)
    {
        if (a == 0) return { i.x,  i.y,  i.z};
        if (a == 1) return { i.x,  i.z, -i.y};
        if (a == 2) return { i.x, -i.y, -i.z};
        if (a == 3) return { i.x, -i.z,  i.y};
        return { 0,0,0 };
    }
    
    point_3d transform(size_t a, const point_3d i)
    {
        auto s = a / 4;
        if (s == 0) return transform_x(a % 4, {  i.x,  i.y,   i.z});
        if (s == 1) return transform_x(a % 4, {  i.z,  i.y,  -i.x });
        if (s == 2) return transform_x(a % 4, { -i.x,  i.y,  -i.z });
        if (s == 3) return transform_x(a % 4, { -i.z,  i.y,   i.x });
        if (s == 4) return transform_x(a % 4, { -i.y,  i.x,   i.z });
        if (s == 5) return transform_x(a % 4, {  i.y, -i.x,   i.z });
        return { 0,0,0 };
    }

    size_t count_same_points(const std::set<point_3d>& s1, const std::vector<point_3d>& s2, const point_3d diff)
    {
        size_t r = 0;
        for (const auto& p1 : s1)
            for (const auto& p2 : s2)
            {
                if ((p1.x == p2.x + diff.x) && (p1.y == p2.y + diff.y) && (p1.z == p2.z + diff.z))
                {
                    r++;
                }
            }
        return r;
    }

    std::pair<point_3d, int64_t> find_fit(const std::set<point_3d>& fix, const std::vector<point_3d>& adapt)
    {
        for (size_t i = 0; i < 24; i++)
        {
            std::vector<point_3d> tmp_adapt(adapt.size());
            for (size_t t = 0; t < adapt.size(); t++)
            {
                tmp_adapt[t] = transform(i, adapt[t]);
            }
            for (const auto& p1 : fix)
            {
                for (const auto& p : tmp_adapt)
                {
                    point_3d diff = {p1.x - p.x, p1.y - p.y, p1.z - p.z };
                    auto hit = count_same_points(fix, tmp_adapt, diff);
                    if (hit >= 12)
                    {
                        return { diff, i };
                    }
                }
            }
        }
        return { {0,0,0}, -1 };
    }

    bool find_configuration(const std::vector<std::vector<point_3d>>& input, std::set<point_3d>& output, std::vector<std::pair<point_3d, int64_t>>& trans, std::set<size_t> available)
    {
        bool found = true;
        while ((available.size() > 0) && (found))
        {
            found = false;
            std::set<size_t> iter = available;
            for (auto a : iter)
            {
                auto r = find_fit(output, input[a]);
                if (r.second != -1)
                {
                    found = true;
                    trans.push_back(r);
                    for (auto const& p : input[a])
                    {
                        auto t_p = transform(r.second, p);
                        output.insert({ t_p.x + r.first.x, t_p.y + r.first.y, t_p.z + r.first.z });
                    }
                    available.erase(a);
                    std::cout << "Remaining: " << available.size() << std::endl;
                }
            }
        }
        return true;
    }

    void run_interal() override
    {
        std::vector<std::vector<point_3d>> scanners;

        while (!input_reader().is_end_of_file())
        {
            int x, y, z;
            auto line = input_reader().get_line();
            if (line.find("scanner") != std::string::npos)
            {
                scanners.push_back(std::vector<point_3d>());
            }
            else if (sscanf_s(line.c_str(), "%d,%d,%d", &x, &y, &z) == 3)
            {
                scanners[scanners.size() - 1].push_back({ x,y,z });
            }
        }

        std::set<point_3d> transformed_set;
        std::vector<std::pair<point_3d, int64_t>> transformations = { {{0,0,0},0} };

        for (auto const& p : scanners[0])
        {
            transformed_set.insert(p);
        }
        
        std::set<size_t>available;
        for (size_t i = 1; i < scanners.size(); i++) available.insert(i);
        auto r = find_configuration(scanners, transformed_set, transformations, available);
        
        set_star1_result(transformed_set.size());

        min_max_counter<int64_t> distance;
        for (const auto& r1 : transformations)
            for (const auto& r2 : transformations)
            {
                distance.check_value(abs(r1.first.x - r2.first.x) + abs(r1.first.y - r2.first.y) + abs(r1.first.z - r2.first.z));
            }

        set_star2_result(distance.maximum());
    }
};
