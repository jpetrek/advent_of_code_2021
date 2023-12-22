#pragma once
#include "helper.h"

template<>
class day<22, 2023> : public day_base<22, 2023>
{   
    struct brick
    {
        std::pair<int64_t,int64_t> x;
        std::pair<int64_t,int64_t> y;
        std::pair<int64_t,int64_t> z;

        bool is_supporting_z(const brick b)
        {
            bool xhit = ((b.x.first< x.first && b.x.second > x.second) || (b.x.first > x.first && b.x.second < x.second) || ((b.x.first >= x.first) && (b.x.first <= x.second)) || ((b.x.second >= x.first) && (b.x.second <= x.second)));
            bool yhit = ((b.y.first< y.first && b.y.second > y.second) || (b.y.first > y.first && b.y.second < y.second) || ((b.y.first >= y.first) && (b.y.first <= y.second)) || ((b.y.second >= y.first) && (b.y.second <= y.second)));
            return xhit && yhit && (z.second == b.z.first - 1);
        }
    };
    
    bool is_test() const override
    {
        return false;
    }
    
    void run_internal() override
    {
        std::vector<std::unique_ptr<brick>> bricks;
        std::map < int64_t, std::pair<std::set<brick*>, std::set<brick*>>> z_buffers;

        while (!input_reader().is_end_of_file())
        {
            auto l = helper::split(input_reader().get_line(), '~');
            auto s = helper::split_and_convert<int64_t>(l[0], ',', [](auto s) {return std::stoll(s); });
            auto e = helper::split_and_convert<int64_t>(l[1], ',', [](auto s) {return std::stoll(s); });
            
            auto b = std::make_unique<brick>();
            b->x = { std::min(s[0], e[0]),std::max(s[0], e[0]) };
            b->y = { std::min(s[1], e[1]),std::max(s[1], e[1]) };
            b->z = { std::min(s[2], e[2]),std::max(s[2], e[2]) };
            if (!z_buffers.contains(b->z.first)) z_buffers[b->z.first] = { {},{} };
            if (!z_buffers.contains(b->z.second)) z_buffers[b->z.second] = { {},{} };
            z_buffers[b->z.first].first.insert(b.get());
            z_buffers[b->z.second].second.insert(b.get());
            bricks.push_back(std::move(b));
        }

        std::map<brick*, std::set<brick*>> supports;
        std::map<brick*, std::set<brick*>> is_supported;

        for (const auto& zs : z_buffers)
        {
            if (zs.first > 1)
            {
                auto backup = zs.second.first;
                for (auto* b : backup)
                {
                    auto z = zs.first;
                    bool free = true;
                    while (free)
                    {
                        for (auto* bellow : z_buffers[z - 1].second)
                        {
                            if (bellow->is_supporting_z(*b))
                            {
                                free = false;
                                if (!supports.contains(bellow)) supports[bellow] = {};
                                supports[bellow].insert(b);
                                if (!is_supported.contains(b)) is_supported[b] = {};
                                is_supported[b].insert(bellow);
                            }
                        }
                        if (free)
                        {
                            z--;
                            z_buffers[b->z.first].first.erase(b);
                            z_buffers[b->z.second].second.erase(b);
                            b->z.second -= b->z.first - z;
                            b->z.first = z;
                            z_buffers[b->z.first].first.insert(b);
                            z_buffers[b->z.second].second.insert(b);
                            if (z == 1) free = false;
                        }
                    }
                }
            }
        }

        std::vector<brick*> dangerous_bricks;
        size_t s1 = 0;
        for (auto& b : bricks)
        {
            if (!supports.contains(b.get())) s1++;
            else
            {
                bool ok = true;
                for (auto* s : supports[b.get()])
                {
                    if (is_supported[s].size() == 1)
                    {
                        ok = false;
                        break;
                    }
                }
                if (ok) s1++;
                else
                {
                    dangerous_bricks.push_back(b.get());
                }
            }
        }
        set_star1_result(s1);
        
        size_t s2 = 0;
        for (auto* b : dangerous_bricks)
        {
            auto supports_copy = supports;
            auto is_supported_copy = is_supported;
            std::queue<brick*> q;
            q.push(b);
            while (!q.empty())
            {
                auto p = q.front(); q.pop();
                if (supports_copy.contains(p))
                {
                    for (auto* s : supports_copy[p])
                    {
                        is_supported_copy[s].erase(p);
                        if (is_supported_copy[s].size() == 0)
                        {
                            s2++;
                            q.push(s);
                        }
                    }
                }
            }
        }
        set_star2_result(s2);
    }
};

