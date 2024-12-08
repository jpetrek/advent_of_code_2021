#pragma once
#include "helper.h"

template<>
class day<2, 2022> : public day_base<2,2022>
{
    const std::vector<size_t> scores = { 3, 6, 0, 0, 3, 6, 6, 0, 3 };
    const std::vector<size_t> moves = { 3, 1, 2, 1, 2, 3, 2, 3, 1 };

    void run_internal() override
    {
        size_t s1 = 0;
        size_t s2 = 0;
        while (!input_reader().is_end_of_file())
        {
            auto n = utility::string::split_and_convert<size_t>(input_reader().get_line(), ' ', [](const std::string& s) { return s[0] < 'D' ? s[0] - 'A' : s[0] - 'X'; });
            s1 += n[1] + 1 + scores[3 * n[0] + n[1]];
            s2 += moves[3 * n[0] + n[1]] + 3 * n[1];
        }
        set_star1_result(s1);
        set_star2_result(s2);
    }
};