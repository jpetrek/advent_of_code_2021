#pragma once
#include "helper.h"

template<>
class day<3, 2022> : public day_base<3,2022>
{
    size_t priority(char c)
    {
        if (std::isupper(c)) return c - 'A' + 27;
        else return c - 'a' + 1;
    }

    void run_interal() override
    {
        size_t s1 = 0;
        size_t s2 = 0;
         std::map<char, size_t> tags;
        std::vector<std::string> group_of_three;
        while (!input_reader().is_end_of_file())
        {
            auto line = input_reader().get_line();
            group_of_three.push_back(line);
            if (group_of_three.size() == 3) 
            {
                char common = ' ';
                for (size_t i = 0; i < group_of_three[0].size(); i++)
                {
                    for (size_t j = 0; j < group_of_three[1].size(); j++)
                    {
                        for (size_t k = 0; k < group_of_three[2].size(); k++)
                        {
                            if ((group_of_three[0][i] == group_of_three[1][j]) && (group_of_three[0][i] == group_of_three[2][k]))
                            {
                                common = group_of_three[0][i];
                            }
                        }
                    }
                }
                group_of_three.clear();
                s2 += priority(common);
            }
            
            char common = ' ';
            for (size_t i = 0; i < line.size() / 2; i++)
            {
                for (size_t j = line.size() / 2; j < line.size(); j++)
                {
                    if (line[i] == line[j])
                    {
                        common = line[i];
                    }
                }
            }
            s1 += priority(common);
        }
        set_star1_result(s1);
        set_star2_result(s2);
    }
};