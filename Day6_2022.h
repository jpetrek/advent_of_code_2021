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
                for (auto i = std::begin(line) + length; i < std::end(line); i++)
                {
                    if  (std::set<char>(i - length, i).size() == length)
                    {
                        if (stars == 0) set_star1_result(std::distance(std::begin(line),i));
                        else set_star2_result(std::distance(std::begin(line), i));
                        break;
                    }
                }
            }
        }
};