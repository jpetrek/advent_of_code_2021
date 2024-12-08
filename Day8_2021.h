#pragma once
#include "helper.h"

using namespace utility::arrays;
using namespace utility::geometry;
using namespace utility::math;
using namespace utility::string;

template<>
class day<8, 2021> : public day_base<8, 2021>
{
    const std::vector<std::string> patterns = { "abcefg","cf","acdeg","acdfg","bcdf","abdfg","abdefg","acf","abcdefg","abcdfg" };

    std::string remap_string(const std::string& input, const std::map<char, char> mapping)
    {
        std::string mapped_string;
        for (auto c : input) mapped_string.push_back(mapping.at(c));
        return mapped_string;
    }

    std::map<char, char> find_mapping_by_permutation(std::vector < std::string> input)
    {
        std::map<char, char> ret_map;
        std::string tmp = "";
        permute("abcdefg", tmp, [&](const std::string& map)->bool
            {
                std::map<char, char> local_mapping = { {map[0],'a'},{map[1],'b'},{map[2],'c'},{map[3],'d'},{map[4],'e'},{map[5],'f'},{map[6],'g'} };

                if (std::all_of(std::begin(input), std::end(input),
                    [&](const auto& in)->bool
                    {
                        std::string mapped_string = remap_string(in, local_mapping);
                        return std::any_of(std::begin(patterns), std::end(patterns),
                            [&](const auto& pattern)->bool
                            {
                                return strings_are_similar(pattern, mapped_string);
                            });
                    }))
                {
                    ret_map = local_mapping;
                    return true;
                }
                    return false;
            });
        return ret_map;
    }

    std::string get_common_characters(const std::vector<std::string>& s)
    {
        std::string ret;
        size_t minimal = 0;
        for (size_t i = 0; i < size(s); i++)
        {
            if (s[i].size() < s[minimal].size())
            {
                minimal = i;
            }
        }

        for (auto c : s[minimal])
        {
            if (std::all_of(std::begin(s), std::end(s),
                [&](const auto& o)->bool
                {
                    return o.find(c) != std::string::npos;
                }))
            {
                ret.push_back(c);
            }
        }
        return ret;
    }

    std::vector<std::string> get_strings_of_length(const std::vector<std::string>& input, const size_t length)
    {
        std::vector<std::string> ret_value;
        for (const auto& s : input)
        {
            if (s.size() == length) ret_value.push_back(s);
        }
        return ret_value;
    }

    std::string string_minus_characters_in_other(const std::string& input, const std::string& second)
    {
        std::string ret;
        for (auto c : input)
        {
            if (second.find(c) == std::string::npos)
            {
                ret.push_back(c);
            }
        }
        return ret;
    }

    std::map<char, char> find_mapping_logically(std::vector < std::string> input)
    {
        std::map<char, char> mapping;

        auto string_8 = get_strings_of_length(input, 7)[0];
        auto string_1 = get_strings_of_length(input, 2)[0];
        auto string_7 = get_strings_of_length(input, 3)[0];
        auto string_4 = get_strings_of_length(input, 4)[0];
        auto common906 = get_common_characters(get_strings_of_length(input, 6));
        auto common523 = get_common_characters(get_strings_of_length(input, 5));

        auto string_a = string_minus_characters_in_other(string_7, string_1);
        auto string_c = string_minus_characters_in_other(string_1, common906);
        auto string_f = string_minus_characters_in_other(string_1, string_c);
        auto string_b = string_minus_characters_in_other(string_minus_characters_in_other(common906, common523), string_1);
        auto string_d = string_minus_characters_in_other(common523, common906);
        auto string_g = string_minus_characters_in_other(string_minus_characters_in_other(common523, string_a), string_d);
        auto string_e = string_minus_characters_in_other(string_minus_characters_in_other(string_minus_characters_in_other(string_8, common523), string_1), string_b);

        mapping[string_a[0]] = 'a';
        mapping[string_b[0]] = 'b';
        mapping[string_c[0]] = 'c';
        mapping[string_d[0]] = 'd';
        mapping[string_e[0]] = 'e';
        mapping[string_f[0]] = 'f';
        mapping[string_g[0]] = 'g';

        return mapping;
    }
    
    void run_internal() override
    {
        size_t sum_star1 = 0;
        size_t sum_star2 = 0;

        while (!input_reader().is_end_of_file())
        {
            auto twoparts = split(input_reader().get_line(), '|');
            auto logged_values = split(twoparts[0], ' ');
            auto input_values = split(twoparts[1], ' ');


            auto mapping = find_mapping_logically(logged_values);
            //auto test = find_mapping_by_permutation(logged_values);

            size_t value = 0;
            for (auto in : input_values)
            {
                std::string mapped_string = remap_string(in, mapping);
                for (size_t pi = 0; pi < patterns.size(); pi++)
                {
                    if (strings_are_similar(patterns[pi], mapped_string))
                    {
                        value *= 10;
                        value += pi;
                        if ((pi == 1) || (pi == 4) || (pi == 7) || (pi == 8)) sum_star1++;
                    }
                }
            }
            sum_star2 += value;
        }

        set_star1_result(sum_star1);
        set_star2_result(sum_star2);
    }
};
