#pragma once
#include "helper.h"

template<>
class day<2, 2024> : public day_base<2,2024>
{
    inline bool is_test() const override { return true; }
    void run_internal() override
    {
         while (!input_reader().is_end_of_file())
        {
        }
//        set_star1_result(sum);
//        set_star2_result(sum);
    }
};