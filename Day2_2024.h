#pragma once
#include "helper.h"

template<>
class day<2, 2024> : public day_base<2,2024>
{
    bool is_correct_series(const std::vector<long>& data, const std::set<long> possible_differences)
    {
        for (size_t i = 0; i < data.size() - 1; i++)
        {
            if (!possible_differences.contains(data[i] - data[i + 1])) return false;
        }
        return true;
    }
    
    bool is_valid(const std::vector<long>& data)
    {
        return is_correct_series(data, { -1,-2,-3 }) || is_correct_series(data, { 1, 2, 3 });
    }
    
    bool is_valid_even_with_one_error(const std::vector<long>& data)
    {
        for (auto i = 0; i < data.size(); i++)
        {
            auto copy = data;
            copy.erase(copy.begin() + i);
            if (is_valid(copy)) return true;
        }
        return false;
    }

    inline bool is_test() const override { return false; }
    void run_internal() override
    {
        size_t count_1 = 0;
        size_t count_2 = 0;
        while (!input_reader().is_end_of_file())
        {
            auto numbers = utility::string::split_convert_vector<long>(input_reader().get_line(), ' ', utility::string::stol());
            if (is_valid(numbers)) count_1++;
            if (is_valid_even_with_one_error(numbers)) count_2++;
        }
        set_star1_result(count_1);
        set_star2_result(count_2);
    }
};