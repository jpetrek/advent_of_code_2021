#pragma once
#include "helper.h"

template<>
class day<2, 2021> : public day_base<2, 2021>
{
    void run_internal() override
    {
        enum eDirection
        {
            eForward,
            eUp,
            eDown
        };

        std::vector<std::pair < eDirection, size_t >> directions;

        while (!input_reader().is_end_of_file())
        {
            std::istringstream iss(input_reader().get_line());
            std::string s;
            unsigned int a;
            iss >> s >> a;
            if (s == "forward") directions.push_back({ eForward , a });
            if (s == "up") directions.push_back({ eUp , a });
            if (s == "down") directions.push_back({ eDown , a });
        }

        size_t hPos = 0;
        size_t vPos = 0;

        for (const auto& i : directions)
        {
            if (i.first == eForward) hPos += i.second;
            if (i.first == eUp) vPos -= i.second;
            if (i.first == eDown) vPos += i.second;
        }
    
        set_star1_result(hPos* vPos);

        hPos = 0;
        size_t aim = 0;
        vPos = 0;

        for (const auto& i : directions)
        {
            if (i.first == eForward)
            {
                hPos += i.second;
                vPos += (aim * i.second);
            }
            if (i.first == eUp) aim -= i.second;
            if (i.first == eDown) aim += i.second;
        }

        set_star2_result(hPos* vPos);
    }
};
