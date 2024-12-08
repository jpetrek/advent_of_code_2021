#pragma once
#include "helper.h"

template<>
class day<5, 2024> : public day_base<5,2024>
{
    typedef std::set<std::pair<long, long>> rules;

    inline bool is_test() const override { return false; }
    void run_internal() override
    {
        rules rules_for_comparison;
        long sum_1 = 0;
        long sum_2 = 0;

        input_reader().read_by_line_until_condition_met_or_eof<file_reader::read_conditions::empty_line>([&](const auto& l)
            {
                rules_for_comparison.insert(helper::vector_to_pair<long>(helper::split_convert_vector<long>(l, '|', [](const auto& in) { return stol(in); })));
            });

        input_reader().read_by_line_until_condition_met_or_eof<file_reader::read_conditions::empty_line>([&](const auto& l)
            {
                auto numbers = helper::split_convert_vector<long>(l, ',', [](const auto& in) { return stol(in); });
                auto backup = numbers;
                std::sort(std::begin(numbers), std::end(numbers), [&](const auto a, const auto b)
                    {
                        return rules_for_comparison.contains({ a,b });
                    });

                if (numbers == backup)
                {
                    sum_1 += numbers[numbers.size() / 2];
                }
                else
                {
                    sum_2 += numbers[numbers.size() / 2];
                }
            });

        set_star1_result(sum_1);
        set_star2_result(sum_2);
    }
};