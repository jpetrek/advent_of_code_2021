#pragma once
#include "helper.h"

template<>
class day<10, 2025> : public day_base<10,2025>
{
    inline bool is_test() const override { return true; }
    void run_internal() override
    {
        size_t sum1 = 0;
        size_t sum2 = 0;
        input_reader().read_by_line_until_condition_met_or_eof<size_t>(file_reader::no_condition(), [&](const auto& line, const auto)
            {
            });


        set_star1_result(sum1);
        set_star2_result(sum2);
    }
};