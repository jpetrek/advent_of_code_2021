#pragma once
#include "helper.h"

template<>
class day<19, 2023> : public day_base<19, 2023>
{   
    struct variable
    {
        enum name
        {
            x = 0,
            m = 1,
            a = 2,
            s = 3
        };

        static name get_name(char n)
        {
            if (n == 'x') return name::x;
            else if (n == 'm') return name::m;
            else if (n == 'a') return name::a;
            else if (n == 's') return name::s;
            throw;
        }
    };

    struct single_values
    {
        int64_t get(variable::name n) const
        {
            return data[static_cast<size_t>(n)];
        }
       
        void set(const variable::name n, const int64_t val)
        {
            data[static_cast<size_t>(n)] = val;
        }

        int64_t sum() const
        {
            return data[0] + data[1] + data[2] + data[3];
        }
    private:
        std::vector<int64_t> data = { 0,0,0,0 };
    };

    struct intervals
    {
        std::pair<int64_t, int64_t> get(const variable::name n) const
        {
            return data.at(static_cast<size_t>(n));
        }

        void set(const variable::name n, const std::pair<int64_t, int64_t> val)
        {
            data[static_cast<size_t>(n)] = val;
        }

        int64_t sum() const
        {
            return (data[0].second - data[0].first + 1) * (data[1].second - data[1].first + 1) * (data[2].second - data[2].first + 1) * (data[3].second - data[3].first + 1);
        }
    private:
        std::vector<std::pair<int64_t, int64_t>> data = { {1,4000}, {1,4000}, {1,4000} , {1,4000} };
    };

    struct condition
    {
        bool lower = false;
        int64_t value;
        std::string target;
        
        inline bool met(int64_t v) const { return lower ? v < value : v > value; }
        
        std::vector<std::pair<bool, std::pair<int64_t, int64_t>>> split_interval_according_to_condition(std::pair<int64_t, int64_t> v) const
        {
            if (lower)
            {
                if (value > v.second) return { {true, v} };
                if (value < v.first) return { {false, v } };
                return { {true, {v.first, value - 1} }, {false, {value,{v.second}} } };

            }
            else
            {
                if (value < v.first) return { {true, v} };
                if (value > v.second) return { {false, v} };
                return { {true, {value + 1,{v.second}} } , {false, {v.first, value} } };
            }
        }
    };

    struct node
    {
        std::vector<std::pair<variable::name, condition>> conditions;
        std::string else_target;
    };

    size_t calculate_star1(const std::map<std::string, node>& nodes, const std::vector<single_values>& data)
    {
        size_t sum = 0;
        for (auto d : data)
        {
            std::string pos = "in";
            while (pos != "A" && pos != "R")
            {
                auto n = nodes.at(pos);
                pos = n.else_target;
                for (const auto c : n.conditions)
                {
                    if (c.second.met(d.get(c.first)))
                    {
                        pos = c.second.target;
                        break;
                    }
                }
            }
            if (pos == "A")
            {
                sum += d.sum();
            }
        }
        return sum;
    }

    size_t calculate_star2(const std::map<std::string, node>& nodes)
    {
        size_t sum = 0;
        
        std::queue<std::pair<std::string, intervals>> vals;
        vals.push({ "in", {} });

        while (!vals.empty())
        {
            auto v = vals.front(); vals.pop();
            if (v.first == "A")
            {
                sum += v.second.sum();
            }
            else
            {
                if (v.first != "R")
                {
                    auto n = nodes.at(v.first);
                    for (const auto c : n.conditions)
                    {
                        auto split = c.second.split_interval_according_to_condition(v.second.get(c.first));
                        if ((split.size() == 1) && (split[0].first))
                        {
                            v.first = c.second.target;
                            vals.push(v);
                            break;
                        }
                        else if (split.size() == 2)
                        {
                            auto nv = v;
                            nv.first = c.second.target;
                            nv.second.set(c.first, split[0].second);
                            vals.push(nv);

                            v.second.set(c.first, split[1].second);
                        }
                    }
                    v.first = n.else_target;
                    vals.push(v);
                }
            }
        }
        return sum;
    }

    std::vector<single_values> parse_input_for_data()
    {
        std::vector<single_values> data;
        while (!input_reader().is_end_of_file())
        {
            auto vars = helper::double_split(helper::trim(input_reader().get_line(),1,1), ',', '=');
            single_values val;
            for (const auto v : vars)
            {
                val.set(variable::get_name(v.first[0]), std::stoll(v.second));
            }
            data.push_back(val);
        }
        return data;
    }

    std::map<std::string, node> parse_input_for_nodes()
    {
        std::map<std::string, node> nodes;
        while (!input_reader().is_end_of_file())
        {
            auto l = input_reader().get_line();
            if (l.empty()) break;

            auto def = helper::split_to_two(l, '{');

            node new_node;
            auto conditions = helper::double_split(helper::trim(def.second, 1, 1), ',', ':');
            for (auto c : conditions)
            {
                if (!c.second.empty())
                {
                    condition cond;
                    cond.target = c.second;
                    if (contains(c.first, '<'))
                    {
                        auto pp = helper::split(c.first, '<');
                        cond.lower = true;
                        cond.value = std::stoll(pp[1]);
                        new_node.conditions.push_back({ variable::get_name(pp[0][0]), cond });
                    }
                    if (contains(c.first, '>'))
                    {
                        auto pp = helper::split(c.first, '>');
                        cond.lower = false;
                        cond.value = std::stoll(pp[1]);
                        new_node.conditions.push_back({ variable::get_name(pp[0][0]), cond });
                    }
                }
                else
                {
                    new_node.else_target = c.first;
                }
            }
            nodes[def.first] = new_node;
        }
        return nodes;
    }

    bool is_test() const override
    {
        return false;
    }

    void run_internal() override
    {
        auto nodes = parse_input_for_nodes();
        auto single_data = parse_input_for_data();

        set_star1_result(calculate_star1(nodes, single_data));
        set_star2_result(calculate_star2(nodes));
    }
};
