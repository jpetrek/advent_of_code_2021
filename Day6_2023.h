#pragma once
#include "helper.h"

template<>
class day<6, 2023> : public day_base<6,2023>
{
        size_t calc(size_t t, size_t d)
        {
            double D = t * t - 4.0 * d;
            auto  r1 = static_cast<size_t>(ceil(((static_cast<double>(t) + sqrt(D)) / 2.0) - 1.0));
            auto  r2 = static_cast<size_t>(floor(((static_cast<double>(t) - sqrt(D)) / 2.0) + 1.0));
            return  r1 - r2 + 1;
        }

        size_t join_input(const std::vector<size_t>& input)
        {
            std::stringstream ss; 
            for (const auto& i : input) ss << i;
            return stoull(ss.str());
        }

        void run_internal() override
        {
        
            auto times = helper::split_and_convert<size_t>(helper::split(input_reader().get_line(), ':')[1], ' ', [](const std::string& a) {return stoull(a); });
            auto distances = helper::split_and_convert<size_t>(helper::split(input_reader().get_line(), ':')[1], ' ', [](const std::string& a) {return stoull(a); });
            size_t s1 = 1;
            for (size_t i = 0; i < times.size(); i++)
            {
                s1 *= calc(times[i], distances[i]);
            }
            set_star1_result(s1);
            set_star2_result(calc(join_input(times), join_input(distances)));
        }
};