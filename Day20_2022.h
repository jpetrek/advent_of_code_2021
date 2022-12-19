#pragma once
#include "helper.h"

template<>
class day<20, 2022> : public day_base<20, 2022>
{   
    void run_internal() override
    {
        while (!input_reader().is_end_of_file())
        {
            auto l = input_reader().get_line();
        }
    }
};

