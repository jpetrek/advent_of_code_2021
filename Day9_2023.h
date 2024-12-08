#pragma once
#include "helper.h"

template<>
class day<9, 2023> : public day_base<9,2023>
{
    void run_internal() override
    {
        size_t sum_s1 = 0;
        size_t sum_s2 = 0;
        while (!input_reader().is_end_of_file())
        {
            auto in = utility::string::split_and_convert<long>(input_reader().get_line(), ' ', utility::string::stol());
            long sum_last = 0;
            long sum_first = 0;
            long coef = 1;
            size_t s = in.size();
            while (!std::all_of(in.begin(), in.begin() + s, utility::math::is_zero<long>()))
                {
                sum_last += in[s - 1];
                sum_first += in[0] * coef;
                coef *= -1;
                
                for (size_t i = 0; i < (s-1); i++)
                {
                    in[i] = in[i + 1] - in[i];
                }

                s--;
            }
            sum_s1 += sum_last;
            sum_s2 += sum_first;
        }

        set_star1_result(sum_s1);
        set_star2_result(sum_s2);
    }
};