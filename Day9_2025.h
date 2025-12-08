    #pragma once
#include "helper.h"

using namespace utility::io;

template<>
class day<9, 2025> : public day_base<9,2025>
{
    inline bool is_test() const override { return true; }
   
    void run_internal() override
    {
        input_reader().read_by_line_until_condition_met_or_eof<size_t>(file_reader::no_condition(), [&](const auto& line, const auto)
        {
        });
        set_star1_result(0);
        set_star2_result(0);
    }   
};