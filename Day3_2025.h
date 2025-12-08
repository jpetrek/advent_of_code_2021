#pragma once
#include "helper.h"

size_t max_number(const std::string& input, size_t requested_digits)
{
    size_t result = 0;
    std::pair<int, long> biggest_with_position_old = { -1, -1 };
    while (requested_digits > 0)
    {
        std::pair<int, long> biggest_with_position_new = { -1, -1 };
        for (int i = static_cast<int>(input.size()  - requested_digits) ; i > biggest_with_position_old.second; i--)
        {
            long cur = input[i] - '0';
            if (biggest_with_position_new.first <= cur)
            {
                biggest_with_position_new = { cur, i };
            }
        }
        requested_digits--;
        result = result * 10 + biggest_with_position_new.first;
        biggest_with_position_old = biggest_with_position_new;
    }
    return result;
}

template<>
class day<3, 2025> : public day_base<3,2025>
{
    inline bool is_test() const override { return false; }
    void run_internal() override
    {
        size_t count_1 = 0;
        size_t count_2 = 0;
        while (!input_reader().is_end_of_file())
        {
            auto line = input_reader().get_line();
            count_1 += max_number(line, 2);
            count_2 += max_number(line, 12);
        }

        set_star1_result(count_1);
        set_star2_result(count_2);
    }
};