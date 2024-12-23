#pragma once
#include "helper.h"

template<>
class day<4, 2022> : public day_base<4,2022>
{
    void run_internal() override
    {
        size_t s1 = 0;
        size_t s2 = 0;
        while (!input_reader().is_end_of_file())
        {
            auto pairs = utility::string::split(input_reader().get_line(), ',');
            auto [p1_1, p1_2] = utility::arrays::vector_to_tuple<2, size_t>(utility::string::split_and_convert<size_t>(pairs[0], '-', [](const auto& s) {return std::stoull(s); }));
            auto [p2_1, p2_2] = utility::arrays::vector_to_tuple<2, size_t>(utility::string::split_and_convert<size_t>(pairs[1], '-', [](const auto& s) {return std::stoull(s); }));
            
            if ((p1_1 >= p2_1) && (p1_2 <= p2_2)) s1++;
            else if ((p2_1 >= p1_1) && (p2_2 <= p1_2)) s1++;
            
            if ((p2_1 >= p1_1) && (p2_1 <= p1_2)) s2++;
            else if ((p1_1 >= p2_1) && (p1_1 <= p2_2)) s2++;
        }
        set_star1_result(s1);
        set_star2_result(s2);
    }
};