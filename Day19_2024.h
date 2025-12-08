#pragma once
#include "helper.h"

using namespace utility::io;

template<>
class day<19, 2024> : public day_base<19,2024>
{
    typedef std::map<std::pair<size_t, size_t>, size_t> cache;

    struct tree_node
    {
        char c;
        bool finish;
        std::map<char, size_t> followers;
    };
    typedef std::vector<tree_node> pattern_tree;

    pattern_tree generate_pattern_tree(std::vector<std::string>& patterns)
    {
        pattern_tree tree = { {'#', false, {}} };
        for (auto const pattern : patterns)
        {
            size_t actual_tree_position = 0;
            for (size_t i = 0; i < pattern.size(); i++)
            {
                if (!tree[actual_tree_position].followers.contains(pattern[i]))
                {
                    tree.push_back({ pattern[i], false, {} });
                    tree[actual_tree_position].followers[pattern[i]] = tree.size() - 1;
                }
                actual_tree_position = tree[actual_tree_position].followers.at(pattern[i]);
            }
            tree[actual_tree_position].finish = true;
        }
        return tree;
    }

    size_t try_generate(const std::string& input, size_t index, size_t tree_index, const pattern_tree& hit_tree, cache& hit_cache)
    {
        std::function<size_t(size_t)> modify_hit_cache_and_return_cached_value = [&](size_t n)->size_t
        { 
            modify_map(hit_cache, { index, tree_index }, 0, [n](auto& v) { v += n; });
            return hit_cache[{index, tree_index}];
        };

        if (hit_cache.contains({ index, tree_index })) return hit_cache.at({ index, tree_index });

        if (index == input.size() || (!hit_tree.at(tree_index).followers.contains(input[index]))) return modify_hit_cache_and_return_cached_value(0);

        auto follower = hit_tree.at(tree_index).followers.at(input[index]);
        if (hit_tree[follower].finish)
        {
            if (index == (input.size() - 1)) return modify_hit_cache_and_return_cached_value(1);
            return modify_hit_cache_and_return_cached_value(try_generate(input, index + 1, 0, hit_tree, hit_cache) + try_generate(input, index + 1, follower, hit_tree, hit_cache));
        }
        else
        {
            return modify_hit_cache_and_return_cached_value(try_generate(input, index + 1, follower, hit_tree, hit_cache));
        }
    }

    inline bool is_test() const override { return false; }
    void run_internal() override
    {
        std::vector<std::string> lookups;
        std::vector<std::string> patterns;

        input_reader().read_by_line_until_condition_met_or_eof<size_t>(file_reader::empty_line_condition(), [&](const auto& l, auto)
        {
            patterns = split(l, ',');
            for (auto& p : patterns) p = trim(p);
        });
        
        input_reader().read_by_line_until_condition_met_or_eof<size_t>(file_reader::empty_line_condition(), [&](const auto& l, auto)
        {
            lookups.push_back(l);
        });

        auto tree = generate_pattern_tree(patterns);

        size_t count = 0;
        size_t count_2 = 0;
        for (const auto s : lookups)
        {
            cache hit_cache;
            auto res  = try_generate(s, 0, 0, tree, hit_cache);
            if (res > 0) count++;
            count_2 += res;
        }
        
        set_star1_result(count);
        set_star2_result(count_2);
    }
};