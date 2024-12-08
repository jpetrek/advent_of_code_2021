#pragma once
#include "helper.h"

using namespace utility::arrays;
using namespace utility::geometry;
using namespace utility::math;
using namespace utility::string;

template<>
class day<6, 2021> : public day_base<6, 2021>
{
    void run_internal() override
    {
        auto ages = split_and_convert<size_t>(input_reader().get_line(), ',');

        std::vector<size_t> histogram_of_ages(9, 0);

        for (auto age : ages)
        {
            histogram_of_ages[age]++;
        }

        for (size_t time_part = 0; time_part < 256; time_part++)
        {
            if (time_part == 80)
            {
                set_star1_result(std::accumulate(histogram_of_ages.begin(), histogram_of_ages.end(), size_t(0)));
            }

            auto zero = histogram_of_ages[0];
            for (size_t i = 0; i < histogram_of_ages.size() - 1; i++)
            {
                histogram_of_ages[i] = histogram_of_ages[i + 1];
            }
            histogram_of_ages[8] = zero;
            histogram_of_ages[6] += zero;
        }

        set_star2_result(std::accumulate(histogram_of_ages.begin(), histogram_of_ages.end(), size_t(0)));
    }
};
