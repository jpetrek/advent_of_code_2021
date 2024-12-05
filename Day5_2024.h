#pragma once
#include "helper.h"

template<>
class day<5, 2024> : public day_base<5,2024>
{
    typedef std::map<long, std::set<long>> map_rules;

    inline bool is_test() const override { return false; }
    void run_internal() override
    {
        map_rules rules;
        long sum_1 = 0;
        long sum_2 = 0;

        std::string line;
        while ((line = input_reader().get_line()) != "")
        {
            auto numbers = helper::split_convert_vector<long>(line, '|', [](const auto& in) { return stol(in); });
            helper::modify_map<map_rules>(rules, numbers[0], {}, [&](auto& s) {s.insert(numbers[1]); });
        }

        while (!input_reader().is_end_of_file())
        {
            auto numbers = helper::split_convert_vector<long>(input_reader().get_line(), ',', [](const auto& in) { return stol(in); });
            auto backup = numbers;
            std::sort(std::begin(numbers), std::end(numbers), [&](const auto a, const auto b)
                {
                    return (rules.find(a) != std::end(rules)) && (rules[a].find(b) != std::end(rules[a]));
                });

            if (numbers == backup)
            {
                sum_1 += numbers[numbers.size() / 2];
            }
            else
            {
                sum_2 += numbers[numbers.size() / 2];
            }
        }

        set_star1_result(sum_1);
        set_star2_result(sum_2);
    }
};