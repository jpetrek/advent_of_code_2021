#pragma once
#include "helper.h"

template<>
class day<6, 2022> : public day_base<6,2022>
{
        void run_interal() override
        {
            auto line = input_reader().get_line();
            for (size_t stars = 0; stars < 2; stars++)
            {
                size_t length = stars == 0 ? 4 : 14;
                for (size_t i = length-1; i++; i < line.size())
                {
                    std::set<char> diff;
                    for (size_t j = 0; j < length; j++)
                    {
                        diff.insert(line[i - j]);
                    }
                    if (diff.size() == length)
                    {
                        if (stars == 0) set_star1_result(i + 1);
                        else set_star2_result(i + 1);
                        break;
                    }
                }
            }
        }
};