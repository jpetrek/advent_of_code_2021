#pragma once
#include "helper.h"

using namespace utility::io;

template<>
class day<19, 2024> : public day_base<19,2024>
{
    typedef std::map<std::pair<size_t, size_t>, std::pair<bool, size_t>> cache;

    struct node
    {
        char c;
        bool fin;
        std::map<char, size_t> followers;
    };
    

    std::vector<node> generate_pattern_tree(std::vector<std::string>& patterns)
    {
        std::vector<node> tree = { {'#', false, {}} };
        for (auto const pattern : patterns)
        {
            auto t = trim(pattern);
            
            size_t pos = 0;
            for (size_t i = 0; i < t.size(); i++)
            {
                if (!tree[pos].followers.contains(t[i]))
                {
                    tree.push_back({ t[i], false, {} });
                    tree[pos].followers[t[i]] = tree.size() - 1;
                }
                pos = tree[pos].followers.at(t[i]);
            }
            tree[pos].fin = true;
        }
        return tree;
    }

    std::pair<bool, size_t> try_create(const std::string& input, size_t index, size_t tree_index, const std::vector<node>& hit_tree, cache& hit_cache)
    {
        std::function<std::pair<bool,size_t>(bool, size_t)> modify_hit_cache = [&](bool v, size_t n)->std::pair<bool, size_t> 
            { 
                if (!hit_cache.contains({ index, tree_index }))
                {
                    hit_cache[{index, tree_index}] = { v,0 };
                }
                hit_cache[{index, tree_index}].second+=n; 
                return hit_cache[{index, tree_index}];
            };

        if (hit_cache.contains({ index, tree_index })) return hit_cache.at({ index, tree_index });

        if (index == input.size() || (!hit_tree.at(tree_index).followers.contains(input[index]))) return modify_hit_cache(false, 0);

        auto f = hit_tree.at(tree_index).followers.at(input[index]);
        if (hit_tree[f].fin)
        {
            if (index == (input.size() - 1))
            {
                // solution has been found!
                return modify_hit_cache(true, 1);
            }
            
            auto v1 = try_create(input, index + 1, 0, hit_tree, hit_cache);
            auto v2 = try_create(input, index + 1, f, hit_tree, hit_cache);
            return modify_hit_cache(v1.first || v2.first, v1.second + v2.second);
        }
        else
        {
            auto v1 = try_create(input, index + 1, f, hit_tree, hit_cache);
            return modify_hit_cache(v1.first, v1.second);
        }
    }

    inline bool is_test() const override { return false; }
    void run_internal() override
    {
        std::vector<std::string> lookups;
        std::vector<std::string> patterns;

        input_reader().read_by_line_until_condition_met_or_eof<size_t>(file_reader::read_condition::empty_line, [&](const auto& l, auto)
        {
            patterns = split(l, ',');
        });
        input_reader().read_by_line_until_condition_met_or_eof<size_t>(file_reader::read_condition::empty_line, [&](const auto& l, auto)
        {
            lookups.push_back(l);
        });

        auto tree = generate_pattern_tree(patterns);


        size_t count = 0;
        size_t count_2 = 0;
        for (const auto s : lookups)
        {
            auto t = trim(s);
            cache hit_cache;
            auto res  = try_create(t, 0, 0, tree, hit_cache);
            std::cout << t << ":" << res.first << ", " << res.second << std::endl;
            if (res.first)
            {
                count++;
                count_2 += res.second;
            }
        }
        

        set_star1_result(count);
        set_star2_result(count_2);
    }
};