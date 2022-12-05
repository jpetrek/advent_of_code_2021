#pragma once
#include "helper.h"

template<>
class day<1, 2022> : public day_base<1,2022>
{
    void run_interal() override
    {
        std::vector<size_t> energy;
        size_t total = 0;
        while (!input_reader().is_end_of_file())
        {
            auto line = input_reader().get_line();
            if (line.empty())
            {
                energy.push_back(total);
                total = 0;
            }
            else
            {
                total += std::stoull(line);
            }
        }
        std::sort(std::begin(energy), std::end(energy));
        set_star1_result(energy[energy.size()-1]);
        set_star2_result(energy[energy.size() - 1]+ energy[energy.size() - 2]+ energy[energy.size() - 3]);
    }
};