#pragma once
#include "helper.h"

template<>
class day<23, 2023> : public day_base<23, 2023>
{
    bool is_test() const override
    {
        return true;
    }
    
    void run_internal() override
    {
        while (!input_reader().is_end_of_file())
        {
            input_reader().get_line();
        }
    }
};
