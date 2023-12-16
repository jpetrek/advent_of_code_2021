#pragma once
#include "helper.h"

template<>
class day<17, 2023> : public day_base<17, 2023>
{
    void run_internal() override
    {
        //set_is_test(true);
        while (!input_reader().is_end_of_file())
        {
            auto line = input_reader().get_line();
        }
        //set_star1_result();
        //set_star2_result();
    }
};
