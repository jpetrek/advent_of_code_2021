#pragma once
#include "helper.h"

template<>
class day<19, 2023> : public day_base<19, 2023>
{   
    bool is_test() const override
    {
        return false;
    }
    void run_internal() override
    {
        while (!input_reader().is_end_of_file())
        {
            auto l = helper::split(input_reader().get_line(), ' ');
        }
        
        //set_star1_result();
        //set_star2_result();
    }
};

