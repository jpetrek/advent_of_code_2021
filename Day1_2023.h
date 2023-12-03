#pragma once
#include "helper.h"

template<>
class day<1, 2023> : public day_base<1,2023>
{
    const std::vector<std::pair<std::string, size_t>> numbers1 =
    {
        {"1",1}, {"2",2}, {"3",3}, {"4",4}, {"5",5}, {"6",6}, {"7",7}, {"8",8}, {"9",9}
    };

    const std::vector<std::pair<std::string, size_t>> numbers2 =
    {
        {"1",1}, {"2",2}, {"3",3}, {"4",4}, {"5",5}, {"6",6}, {"7",7}, {"8",8}, {"9",9},
        {"one",1}, {"two",2}, {"three",3}, {"four",4}, {"five",5}, {"six",6}, {"seven",7}, {"eight",8}, {"nine",9}
    };

    size_t get_number(const std::string& in, const std::vector<std::pair<std::string, size_t>>& digit_definitions) const
    {
        size_t i = 0;
        while (i < in.length())
        {
            std::string_view view_moving_start = std::string_view(in).substr(i, in.length() - i);
            for (const auto& decimal_digit_definition : digit_definitions)
            {
                if (view_moving_start.starts_with(decimal_digit_definition.first))
                {
                    size_t j = 0;
                    while (j < in.length())
                    {
                        std::string_view view_moving_end = std::string_view(in).substr(0, in.length() - j);
                        for (const auto& digit_definition : digit_definitions)
                        {
                            if (view_moving_end.ends_with(digit_definition.first))
                            {
                                return 10 * decimal_digit_definition.second + digit_definition.second;
                            }
                        }
                        j++;
                    }
                }
            }
            i++;
        }
        return 0; 
    }

     void run_internal() override
    {
        summary_counter<size_t> s1;
        summary_counter<size_t> s2;
        while (!input_reader().is_end_of_file())
        {
            auto line = input_reader().get_line();
            s1.add(get_number(line, numbers1));
            s2.add(get_number(line, numbers2));
        }
        set_star1_result(s1.value());
        set_star2_result(s2.value());
    }
};