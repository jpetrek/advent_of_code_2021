#pragma once
#include "helper.h"

template<>
class day<15, 2022> : public day_base<15, 2022>
{
    void run_interal() override
    {
        while (!input_reader().is_end_of_file())
        {
            auto line = input_reader().get_line();
        }
    }
};
