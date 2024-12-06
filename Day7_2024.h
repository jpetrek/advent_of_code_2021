#pragma once
#include "helper.h"

template<>
class day<7, 2024> : public day_base<7,2024>
{
    inline bool is_test() const override { return false; }
    void run_internal() override
    {
        input_reader().read_by_line_until_condition_or_eof<file_reader::read_conditions::empty_line>([&](const auto& l)
            {
            });

//        set_star1_result(sum_1);
//        set_star2_result(sum_2);
    }
};