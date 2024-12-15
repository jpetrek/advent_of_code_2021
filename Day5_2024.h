#pragma once
#include "helper.h"

using namespace utility::io;

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

        input_reader().read_by_line_until_condition_met_or_eof<size_t>(file_reader::read_condition::empty_line, [&](const auto& l, auto)
            {
                rules_for_comparison.insert(utility::arrays::vector_to_pair<long>(utility::string::split_convert_vector<long>(l, '|', utility::string::stol())));
            });

        input_reader().read_by_line_until_condition_met_or_eof<size_t>(file_reader::read_condition::empty_line, [&](const auto& l, auto)
            {
                auto numbers = utility::string::split_convert_vector<long>(l, ',', utility::string::stol());
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