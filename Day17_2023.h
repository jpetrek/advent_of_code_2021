#pragma once
#include "helper.h"

template<>
class day<17, 2023> : public day_base<17, 2023>
{
    bool is_test() const override
    {
        return true;
    }
    
    void run_internal() override
    {
        while (!input_reader().is_end_of_file())
        {
            auto line = input_reader().get_line();
        }
        //set_star1_result();
        //set_star2_result();
    }
};
