#pragma once
#include "helper.h"

using namespace utility::io;

template<>
class day<19, 2024> : public day_base<19,2024>
{

    inline bool is_test() const override { return true; }
    void run_internal() override
    {
        input_reader().read_by_line_until_condition_met_or_eof<size_t>(file_reader::read_condition::empty_line, [&](const auto& l, auto)
            {
            });
        //set_star1_result(sum_1);
        //set_star2_result(sum_2);
    }
};