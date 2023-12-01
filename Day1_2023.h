#pragma once
#include "helper.h"

template<>
class day<1, 2023> : public day_base<1,2023>
{
    const std::vector<std::pair<std::string, size_t>> numbers1 =
    {
        {"1",1}, {"2",2}, {"3",3}, {"4",4}, {"5",5}, {"6",6}, {"7",7}, {"8",8}, {"9",9}
    };

    const std::vector<std::pair<std::string, size_t>> numbers2 =
    {
        {"1",1}, {"2",2}, {"3",3}, {"4",4}, {"5",5}, {"6",6}, {"7",7}, {"8",8}, {"9",9},
        {"one",1}, {"two",2}, {"three",3}, {"four",4}, {"five",5}, {"six",6}, {"seven",7}, {"eight",8}, {"nine",9}
    };

    size_t compare_position_safe(const size_t current_position, const size_t new_position, const std::function<bool(size_t, size_t)>& comp) const
    {
        if (new_position != std::string::npos)
        {
            if ((current_position == std::string::npos) || comp(new_position, current_position)) { return new_position; };
        }
        return current_position;
    }

    size_t extract_number(const std::string& in, const std::vector<std::pair<std::string, size_t>>& substring) const
    {
        std::pair<size_t, size_t> left_min_pos_and_digit = { std::string::npos, 0 };
        std::pair<size_t, size_t> right_max_pos_and_digit = { std::string::npos, 0 };
        for (const auto& number : substring)
        {
            auto new_left_position = compare_position_safe(left_min_pos_and_digit.first, in.find(number.first), [](size_t a, size_t b) {return a < b; });
            if (new_left_position != left_min_pos_and_digit.first)
            {
                left_min_pos_and_digit = { new_left_position , number.second };
            }

            auto new_right_position = compare_position_safe(right_max_pos_and_digit.first, in.rfind(number.first), [](size_t a, size_t b) {return a > b; });
            if (new_right_position != right_max_pos_and_digit.first)
            {
                right_max_pos_and_digit = { new_right_position, number.second };
            }
        }
        return 10 * left_min_pos_and_digit.second + right_max_pos_and_digit.second;
    }

     void run_internal() override
    {
        summary_counter<size_t> s1;
        summary_counter<size_t> s2;
        while (!input_reader().is_end_of_file())
        {
            auto line = input_reader().get_line();
            s1.add(extract_number(line, numbers1));
            s2.add(extract_number(line, numbers2));
        }
        set_star1_result(s1.value());
        set_star2_result(s2.value());
    }
};