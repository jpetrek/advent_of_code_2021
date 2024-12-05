#pragma once
#include "helper.h"

template<>
class day<6, 2024> : public day_base<6,2024>
{
    inline bool is_test() const override { return false; }
    void run_internal() override
    {
        while (!input_reader().is_end_of_file())
        {
            auto numbers = helper::split_convert_vector<long>(input_reader().get_line(), ' ', [](const auto& in) { return stol(in); });
        }
        //set_star1_result(sum);
        //set_star2_result(sum);
    }
};