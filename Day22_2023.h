#pragma once
#include "helper.h"

template<>
class day<22, 2023> : public day_base<22, 2023>
{   
    bool is_test() const override
    {
        return true;
    }
    
    void run_internal() override
    {
    }
};

