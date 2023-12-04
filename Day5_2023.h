#pragma once
#include "helper.h"

template<>
class day<5, 2023> : public day_base<5,2023>
{
        void run_internal() override
        {
            auto line = input_reader().get_line();
            //set_star1_result(take_top_items(positions_star1));
	    //set_star2_result(take_top_items(positions_star2));
        }
};