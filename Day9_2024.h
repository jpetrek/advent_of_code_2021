    #pragma once
#include "helper.h"

using namespace utility::io;

template<>
class day<9, 2024> : public day_base<9,2024>
{
    inline bool is_test() const override { return true; }
    void run_internal() override
    {
         input_reader().read_by_line_until_condition_met_or_eof<file_reader::read_conditions::empty_line>([&](const auto& line)
            {
            });
        //set_star1_result();
        //set_star2_result();
    }
};