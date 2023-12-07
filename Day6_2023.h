#pragma once
#include "helper.h"

template<>
class day<6, 2023> : public day_base<6,2023>
{
        size_t calculate(size_t t, size_t d) const
        {
            auto roots = helper::solve_quadratic_eq(1, static_cast<double>(t), static_cast<double>(d));
            return static_cast<size_t> (ceil(roots[1] - 1.0)) - static_cast<size_t> (floor(roots[0] + 1.0)) + 1;
        }

        size_t join_input(const std::vector<size_t>& input) const
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
                s1 *= calculate(times[i], distances[i]);
            }
            set_star1_result(s1);
            set_star2_result(calculate(join_input(times), join_input(distances)));
        }
};