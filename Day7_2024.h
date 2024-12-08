#pragma once
#include "helper.h"

template<>
class day<7, 2024> : public day_base<7,2024>
{
    inline bool is_test() const override { return false; }

    bool can_calculate_s1(const uint64_t res, const uint64_t actual, size_t actual_index, const std::vector<uint64_t>& numbers) const
    {
        if ((actual == res) && (actual_index == numbers.size())) return true;
        if ((actual > res)  || (actual_index == numbers.size())) return false;
        return can_calculate_s1(res, actual + numbers[actual_index], actual_index + 1, numbers) || can_calculate_s1(res, actual * numbers[actual_index], actual_index + 1, numbers);
    }

    bool can_calculate_s2(const uint64_t res, const uint64_t actual, size_t actual_index, const std::vector<uint64_t>& numbers) const
    {
        if ((actual == res) && (actual_index == numbers.size())) return true;
        if ((actual > res) || (actual_index == numbers.size())) return false;

        uint64_t factor = static_cast<uint64_t>(pow(10, digits10<uint64_t>(numbers[actual_index])));
        return can_calculate_s2(res, actual + numbers[actual_index], actual_index + 1, numbers)
            || can_calculate_s2(res, actual * numbers[actual_index], actual_index + 1, numbers)
            || can_calculate_s2(res, factor * actual + numbers[actual_index], actual_index + 1, numbers);
    }
    
    void run_internal() override
    {
        uint64_t sum1 = 0;
        uint64_t sum2 = 0;
        input_reader().read_by_line_until_condition_met_or_eof<utility::io::file_reader::read_conditions::empty_line>([&](const auto& line)
            {
                auto n = utility::string::split(line, ':');
                auto numbers =utility::string::split_convert_vector<uint64_t>(n[1], ' ', utility::string::stoull());
                uint64_t res = stoull(n[0]);
                
                if (can_calculate_s1(res, numbers[0], 1, numbers)) sum1 += res;
                if (can_calculate_s2(res, numbers[0], 1, numbers)) sum2 += res;
            });
        set_star1_result(sum1);
        set_star2_result(sum2);
    }
};