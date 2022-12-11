#pragma once
#include "helper.h"

template<>
class day<10, 2022> : public day_base<10,2022>
{
    std::vector<int> transform_for_one(const std::vector<std::pair<int, int>>& i)
    {
        std::vector<int> result;
        int r = 1;
        for (const auto ii : i)
        {
            if (ii.first == 0) result.push_back(r);
            if (ii.first == 1)
            {
                result.push_back(r);
                result.push_back(r);
                r += ii.second;
            }
        }
        return result;
    }
    
    void run_interal() override
    {
        std::vector<std::pair<int, int>> code;
        while (!input_reader().is_end_of_file())
        {
            auto in = helper::split(input_reader().get_line(), ' ');
            if (in.size() == 1)
            {
                code.push_back({0,0});
            }
            else if (in.size() == 2)
            {
                code.push_back({ 1, std::stol(in[1]) });
            }
        }

        auto s1r = transform_for_one(code);
        set_star1_result(20 * s1r[19] + 60 * s1r[59] + 100 * s1r[99] + 140 * s1r[139] + 180 * s1r[179] + 220 * s1r[219]);

        for (size_t i = 0; i < 240; i++)
        {
            if (((i % 40) == 0) && i != 0)
            {
                std::cout << std::endl;
            }
            int n = (static_cast<int>(i) / 40)*40 - 1;
            if ((i >= (s1r[i] + n)) && (i < (s1r[i] + 3 + n ))) std::cout << '#';
            else std::cout << '.';
        }
        std::cout << std::endl;
    }
};