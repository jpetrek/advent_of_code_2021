#pragma once
#include "helper.h"

using namespace utility::io;

template<>
class day<5, 2025> : public day_base<5,2025>
{
    typedef std::pair<size_t, size_t> range;
    typedef std::vector<range> ranges;

    inline bool is_test() const override { return false; }
    
   
    size_t count_unique_ranges(ranges input) 
    {
        size_t result = 0;

        std::sort(std::begin(input), std::end(input), [](const auto& a, const auto& b)
            {
                return (a.first != b.first) ? a.first < b.first : a.second < b.second;
            });

        range current = input[0];

        for (std::size_t i = 1; i < input.size(); ++i)
        {
            const range& next = input[i];

            if (next.first <= current.second)
            {
                current.second = std::max(current.second, next.second);
            }
            else
            {
                result += current.second - current.first - 1;
                current = next;
            }
        }

        result += current.second - current.first - 1;
        return result;
    }

    void run_internal() override
    {
        ranges ranges_for_freshness;
        size_t sum_1 = 0;
        size_t sum_2 = 0;


        input_reader().read_by_line_until_condition_met_or_eof<size_t>(file_reader::empty_line_condition(), [&](const auto& l, auto)
            {
                ranges_for_freshness.push_back(vector_to_pair(split_and_convert<size_t>(l, '-', utility::string::stoull())));
            });

        input_reader().read_by_line_until_condition_met_or_eof<size_t>(file_reader::empty_line_condition(), [&](const auto& l, auto)
            {
                size_t v = utility::string::stoull()(l);
                for (const auto& r : ranges_for_freshness)
                {
                    if (is_in_range(r.first, r.second, true, v))
                    {
                        sum_1++;
                        break;
                    }
                }
            });

        set_star1_result(sum_1);
        set_star2_result(count_unique_ranges(ranges_for_freshness));
    }
};