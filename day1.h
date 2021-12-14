#pragma once
#include "helper.h"

class day1 : public day_base<1>
{
    void run_interal() override
    {
        std::vector<unsigned int> numbers;
        while (!input_reader().is_end_of_file())
        {
            std::istringstream iss(input_reader().get_line());
            unsigned int a;
            iss >> a;
            numbers.push_back(a);
        }

        size_t increased = 0;

        for (size_t i = 1; i < numbers.size(); i++)
        {
            if (numbers[i] > numbers[i - 1]) increased++;
        }

        set_star1_result(increased);

        increased = 0;

        for (size_t i = 1; i < numbers.size() - 2; i++)
        {
            size_t s1 = numbers[i - 1] + numbers[i] + numbers[i + 1];
            size_t s2 = numbers[i] + numbers[i + 1] + numbers[i + 2];
            if (s2 > s1) increased++;
        }

        set_star2_result(increased);
    }
};