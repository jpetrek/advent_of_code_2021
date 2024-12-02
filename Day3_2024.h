#pragma once
#include "helper.h"

template<>
class day<3, 2024> : public day_base<3,2024>
{
    inline bool is_test() const override { return true; }
    void run_internal() override
    {
        while (!input_reader().is_end_of_file())
        {
            auto numbers = helper::split_convert_vector<long>(input_reader().get_line(), ' ', [](const auto& in) { return stol(in); });
        }
//        set_star1_result(count_1);
//        set_star2_result(count_2);
    }
};