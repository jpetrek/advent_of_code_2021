#pragma once
#include "helper.h"
using namespace utility::io;

template<>
class day<16, 2024> : public day_base<16,2024>
{
    typedef point_2d_generic<long> position;
    typedef direction_2d_generic<long> direction;
    void run_internal() override
    {
        input_reader().read_by_line_until_condition_met_or_eof<long>(file_reader::read_condition::empty_line, [&](const auto& line, const long y)
        {
        });
        //set_star1_result();
        //set_star2_result();
    }
};