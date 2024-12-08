#pragma once
#include "helper.h"

template<>
class day<1, 2024> : public day_base<1,2024>
{
    inline bool is_test() const override { return false; }
    void run_internal() override
    {
        std::vector<long> numbers1;
        std::vector<long> numbers2;
        std::map<long, long> number_map;

        while (!input_reader().is_end_of_file())
        {
            auto numbers = utility::string::split_convert_vector<long>(input_reader().get_line(), ' ', utility::string::stol());
            number_map[numbers[0]] = 0;
            numbers1.push_back(numbers[0]);
            numbers2.push_back(numbers[1]);
        }

        std::sort(std::begin(numbers1), std::end(numbers1));
        std::sort(std::begin(numbers2), std::end(numbers2));

        long sum = 0;
        for (auto i = 0; i < numbers1.size(); i++)
        {
            sum += abs(numbers1[i] - numbers2[i]);
            number_map[numbers2[i]]++;
        }

        set_star1_result(sum);
       
        sum = 0;
        for (auto i = 0; i < numbers1.size(); i++)
        {
            sum += numbers1[i] * number_map[numbers1[i]];
        }

        set_star2_result(sum);
    }
};