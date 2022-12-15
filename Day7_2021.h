#pragma once
#include "helper.h"

template<>
class day<7, 2021> : public day_base<7, 2021>
{
    void run_internal() override
    {
        auto positions = helper::split_and_convert<size_t>(input_reader().get_line(), ',');

        auto maxPos = *std::max_element(std::cbegin(positions), std::cend(positions));

        min_max_counter<size_t>  minimum_fuel_cost_1;
        min_max_counter<size_t>  minimum_fuel_cost_2;

        for (size_t i = 0; i < maxPos; i++)
        {
            size_t fuel_cost_1 = 0;
            size_t fuel_cost_2 = 0;
            for (auto p : positions)
            {
                size_t dist = abs(static_cast<long>(i) - static_cast<long>(p));
                fuel_cost_1 += dist;
                fuel_cost_2 += dist * (dist + 1) / 2;;
            }
            minimum_fuel_cost_1.check_value(fuel_cost_1);
            minimum_fuel_cost_2.check_value(fuel_cost_2);
        }

        set_star1_result(minimum_fuel_cost_1.minimum());
        set_star2_result(minimum_fuel_cost_2.minimum());
    }
};
