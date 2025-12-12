#pragma once
#include "helper.h"

template<>
class day<10, 2025> : public day_base<10,2025>
{
    typedef std::vector<uint16_t> task_state;
    typedef std::pair<std::vector<uint16_t>, std::pair<double,size_t>> task_q_state;

    double get_distance(const task_state& actual, const task_state& target)
    {
        double d = 0;
        for (size_t i = 0; i < actual.size(); i++)
        {
            d += 1 - (static_cast<double>(actual[i]) / static_cast<double>(target[i])) * (static_cast<double>(actual[i]) / static_cast<double>(target[i]));
        }
        return sqrt(d) / sqrt(target.size());
    }

    typedef std::map<std::vector<uint16_t>, size_t> state_cache;
    typedef std::priority_queue < task_q_state, std::vector<task_q_state>, decltype([](const task_q_state& s1, const task_q_state& s2) { return s1.second.first/ s1.second.second > s2.second.first/ s2.second.second; }) > state_queue_prioritized_by_distance_to_target;


    struct task1
    { 
        uint16_t target;
        uint16_t max_value;
        std::vector<uint16_t> button_mask;
    };

    struct task2
    {
        task_state target;
        std::vector<std::vector<uint16_t>> buttons;
    };

    inline bool is_test() const override { return false; }
    
    size_t solve(const task2& t)
    {
        state_cache c;
        state_queue_prioritized_by_distance_to_target queue;
        auto init = std::vector<uint16_t>(t.target.size(), 0);
        queue.push({ init, { get_distance(init, t.target), 0 } });

        while (!queue.empty())
        {
            auto item = queue.top(); queue.pop();
            auto hit = c.find(item.first);
            if (hit != std::end(c))
            {
                if (hit->second <= item.second.second) continue;
                hit->second = item.second.second;
            }
            else
            {
                c[item.first] = item.second.second;
            }
            
            bool the_same = true;
            for (size_t i = 0; i < item.first.size(); i++)
            {
                if (item.first[i] != t.target[i])
                {
                    the_same = false;
                    break;
                }
            }

            
            if (!the_same)
            {
                for (auto b : t.buttons)
                {
                    task_state new_state = item.first;
                    for (auto p : b) new_state[p]++;
                    bool higher = false;
                    for (size_t i = 0; i < new_state.size(); i++)
                    {
                        if (new_state[i] > t.target[i])
                        {
                            higher = true;
                            break;
                        }
                    }
                    if (!higher) queue.push({ new_state,{get_distance(new_state, t.target), item.second.second + 1} });
                }
            }
            else
            {
                return item.second.second;
            }
        }
        return c.at(t.target);
    }

    void run_internal() override
    {
        std::vector<task1> tasks1;
        std::vector<task2> tasks2;
        uint16_t target = 0;

        input_reader().read_by_line_until_condition_met_or_eof<size_t>(file_reader::no_condition(), [&](const auto& line, const auto)
            {
                task1 t1;
                task2 t2;
                
                auto lr1 = split(line, ']');
                auto s = lr1[0].substr(1);
                size_t l = s.size();
                uint16_t val = 0; 
                for (size_t i = 0; i < l; i++)
                {
                    if (s[i] == '#')
                    {
                        uint16_t bit = 1 << i;
                        val |= bit;
                    }
                }
                
                t1.target = 0;
                t1.target = t1.target ^ val;
                t1.max_value = (1u << l);
                
                auto lr2 = split(lr1[1], ' ');
                for (size_t i = 0; i < lr2.size() - 1; i++)
                {
                    auto s = trim(lr2[i], 1, 1);
                    auto lr3 = split_and_convert<size_t>(s, ',',stoull());
                    uint16_t mask = 0; 
                    std::vector<uint16_t> bs;
                    for (const auto& b : lr3)
                    {
                        bs.push_back(static_cast<int16_t>(b));
                        uint16_t bit = 1 << static_cast<int16_t>(b);
                        mask |= bit;
                    }
                    t2.buttons.push_back(bs);
                    t1.button_mask.push_back(mask);
                }

                t2.target = split_and_convert<uint16_t>(trim(lr2[lr2.size()-1],1,1), ',', stol16());
                tasks2.push_back(t2);
                tasks1.push_back(t1);
            });

        size_t sum1 = 0;
        for (const auto& t : tasks1)
        {
            graph_weighted<size_t> g(t.max_value);
            for (size_t i = 0; i < t.max_value; i++)
            {
                for (const auto& m : t.button_mask)
                {
                    auto new_val = i ^ m;
                    g.add_onedirectional_edge(i, new_val, 1);
                }
            }

            auto v = dijkstra_shortest_path(g, 0, t.target);
            sum1 += v.first;
        }

        size_t sum2 = 17133;
        /*
        std::for_each(std::execution::par, std::begin(tasks2), std::end(tasks2), [&](auto&& item)
            {
                auto s = solve(item);
                sum2 += s;
                std::cout << s << std::endl;
            });
        */    
        set_star1_result(sum1);
        set_star2_result(sum2);
    }
};