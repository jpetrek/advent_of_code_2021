#pragma once
#include "helper.h"
using namespace utility::io;

template<>
class day<8, 2024> : public day_base<8,2024>
{
    typedef point_2d_generic<long> coords;
    typedef direction_2d_generic<long> dif;

    void add_antidote_if_in_limits(const coords c, const dif::diference d, const long max_x, const long max_y, std::set<coords>& antidotes) const
    {
        coords nc = { c.x + d.dx, c.y + d.dy };
        if ((nc.x >= 0) && (nc.x < max_x) && (nc.y >= 0) && (nc.y < max_y)) antidotes.insert(nc);
    }

    void add_antidotes_until_in_limits(const coords c, const dif::diference d, const long max_x, const long max_y, std::set<coords>& antidotes) const
    {
        antidotes.insert(c);
        coords nc = c;
        bool ok = true;
        while (ok)
        {
            nc = { nc.x + d.dx, nc.y + d.dy };
            if ((nc.x >= 0) && (nc.x < max_x) && (nc.y >= 0) && (nc.y < max_y)) antidotes.insert(nc);
            else ok = false;
        }
    }

    inline bool is_test() const override { return false; }
    void run_internal() override
    {
        uint64_t sum1 = 0;
        uint64_t sum2 = 0;
        std::map<char, std::set<coords>> area;
        std::set<coords> antidotes;
        long max_x = 0;
        long max_y = 0;
        input_reader().read_by_line_until_condition_met_or_eof<size_t>(file_reader::empty_line_condition(), [&](const auto& line, const auto)
            {
                max_x = static_cast<long>(line.size());
                for (long x = 0; x < line.size(); x++)
                {
                    if (line[x] != '.')
                    {
                        utility::arrays::modify_map(area, line[x], {}, [&](auto& i) { i.insert({ x,max_y }); });
                    }
                }
                max_y++;
            });
        
        for (const auto& m : area)
        {
            for (const auto p1 : m.second)
            {
                for (const auto p2 : m.second)
                {
                    if (p1 != p2)
                    {
                        add_antidote_if_in_limits(p1, { p1.x - p2.x, p1.y - p2.y }, max_x, max_y, antidotes);
                        add_antidote_if_in_limits(p2, { p2.x - p1.x, p2.y - p1.y }, max_x, max_y, antidotes);
                    }
                }
            }
        }
        set_star1_result(antidotes.size());
        
        antidotes.clear();

        for (const auto& m : area)
        {
            for (const auto p1 : m.second)
            {
                for (const auto p2 : m.second)
                {
                    if (p1 != p2)
                    {
                        add_antidotes_until_in_limits(p1, { p1.x - p2.x, p1.y - p2.y }, max_x, max_y, antidotes);
                        add_antidotes_until_in_limits(p2, { p2.x - p1.x, p2.y - p1.y }, max_x, max_y, antidotes);
                    }
                }
            }
        }
        set_star2_result(antidotes.size());
    }
};