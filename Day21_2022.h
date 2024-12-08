#pragma once
#include "helper.h"

template<>
class day<21, 2022> : public day_base<21, 2022>
{   
    std::function<int64_t(int64_t, int64_t)> empty = [](int64_t a, int64_t b) {return 0; };
    std::function<int64_t(int64_t, int64_t)> plus = [](int64_t a, int64_t b) {return a + b; };
    std::function<int64_t(int64_t, int64_t)> minus = [](int64_t a, int64_t b) {return a - b; };
    std::function<int64_t(int64_t, int64_t)> mult = [](int64_t a, int64_t b) {return a * b; };
    std::function<int64_t(int64_t, int64_t)> divide = [](int64_t a, int64_t b) {return a / b; };

    struct tree_item
    {
        std::string id;
        std::string id_left;
        std::string id_right;
        bool is_just_number;
        int64_t value;
        char f_id;
        std::function<int64_t(int64_t, int64_t)> f;
        tree_item* left;
        tree_item* right;
    };
    
    int64_t Calculate_s1(tree_item* item)
    {
        if (item->is_just_number) return item->value;
        return item->f(Calculate_s1(item->left) , Calculate_s1(item->right));
    }

    std::pair<bool, int64_t> Calculate_s2(tree_item* item, int64_t expected, bool backward)
    {
        if (item->id == "root")
        {
            auto l = Calculate_s2(item->left, expected, false);
            auto r = Calculate_s2(item->right, expected, false);
            if (l.first)
            {
                auto tmp = Calculate_s2(item->right, l.second, true);
                return { tmp.second == l.second, 0 };
            }
            else if (r.first)
            {
                auto tmp  = Calculate_s2(item->left, r.second, true);
                return { tmp.second == r.second, 0 };
            }
        }
        else
        {
            if (item->id == "humn")
            {
                if (backward)
                {
                    set_star2_result(expected);
                    return { true, expected };
                }
                else
                    return { false, item->value };
            }
            
            if (item->is_just_number) return { true, item->value };

            auto l = Calculate_s2(item->left, 0, false);
            auto r = Calculate_s2(item->right, 0, false);

            if (!backward)
            {
                return { l.first && r.first, item->f(l.second, r.second) };
            }

            if (l.first && r.first) return { true, item->f(l.second, r.second) };
            else if (!l.first && !r.first)
            {
                throw; // shall not happen
            }
            else if (!l.first)
            {
                std::pair<bool, int64_t> tmp;
                if (item->f_id == '+')
                {
                    tmp = Calculate_s2(item->left, expected - r.second, true);
                }
                else if (item->f_id == '-')
                {
                    tmp = Calculate_s2(item->left, expected + r.second, true);
                }
                else if (item->f_id == '*')
                {
                    tmp = Calculate_s2(item->left, expected / r.second, true);
                }
                else if (item->f_id == '/')
                {
                    tmp = Calculate_s2(item->left, expected * r.second, true);
                }

                return { true, item->f(tmp.second, r.second) };
            }
            else if (!r.first)
            {
                std::pair<bool, int64_t> tmp;
                if (item->f_id == '+')
                {
                    tmp = Calculate_s2(item->right, expected - l.second, true);
                }
                else if (item->f_id == '-')
                {
                    tmp = Calculate_s2(item->right, l.second - expected, true);
                }
                else if (item->f_id == '*')
                {
                    tmp = Calculate_s2(item->right, expected / l.second, true);
                }
                else if (item->f_id == '/')
                {
                    tmp = Calculate_s2(item->right, l.second / expected, true);
                }

                return { true, item->f(l.second,tmp.second) };
            }
        }
    }

    void run_internal() override
    {
        std::vector<std::unique_ptr<tree_item>> data;
        std::map<std::string, tree_item*> name_map;
        tree_item* root = nullptr;
        while (!input_reader().is_end_of_file())
        {
            auto parts = utility::string::split(input_reader().get_line(), ':');
            auto expr = utility::string::split(parts[1], ' ');
            data.push_back(std::make_unique<tree_item>());
            if (expr.size() == 1)
            {
                data.back()->id = parts[0];
                data.back()->is_just_number = true;
                data.back()->value = std::stoll(expr[0]);
                data.back()->f = empty;
                data.back()->f_id = ' ';
                data.back()->id_left = "";
                data.back()->id_right = "";
                data.back()->left = nullptr;
                data.back()->right = nullptr;
            }
            else
            {
                data.back()->id = parts[0];
                data.back()->is_just_number = false;
                data.back()->value = 0;
                data.back()->id_left = expr[0];
                data.back()->id_right = expr[2];
                data.back()->left = nullptr;
                data.back()->right = nullptr;
                data.back()->f_id = expr[1][0];
                if (expr[1] == "+") data.back()->f = plus;
                if (expr[1] == "-") data.back()->f = minus;
                if (expr[1] == "*") data.back()->f = mult;
                if (expr[1] == "/") data.back()->f = divide;
            }
            name_map[parts[0]] = data.back().get();
        }

        for (auto& i : data)
        {
            if (!i->is_just_number)
            {
                i->left = name_map[i->id_left];
                i->right = name_map[i->id_right];
            }
        }
        root = name_map["root"];
        set_star1_result(Calculate_s1(root));
        Calculate_s2(root, 0, false);
    }
};

