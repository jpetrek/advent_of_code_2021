#pragma once
#include "helper.h"

class day3 : public day_base<3>
{
    void run_interal() override
    {
        std::vector<long>counts = { 0,0,0,0,0,0,0,0,0,0,0,0 };
        std::vector<std::vector<unsigned>> bits;

        while (!input_reader().is_end_of_file())
        {
            std::istringstream iss(input_reader().get_line());
            std::string s;
            iss >> s;
            bits.push_back(std::vector<unsigned>(12, 0));
            for (size_t i = 0; i < s.size(); i++)
            {
                if (s[i] == '1') (*(bits.end() - 1))[i] = 1;
            }

            for (int i = static_cast<int>(s.size()) - 1; i >= 0; i--)
            {
                if (s[i] == '1')
                {
                    counts[i]++;
                }
            }
        }

        long value = 0;
        long mult = 1;
        for (int i = 11; i >= 0; i--)
        {
            if (counts[i] > 500)
            {
                value += mult;
            }
            mult *= 2;
        }

        set_star1_result(value * (~value & 0xFFF));

        std::vector<std::vector<unsigned>> wb = bits;
        size_t i = 0;
        while (wb.size() != 1)
        {
            size_t c1 = 0;
            size_t c0 = 0;
            for (const auto& v : wb)
            {
                if (v[i] == 1) c1++;
                else c0++;
            }

            std::vector<std::vector<unsigned>> nwb;

            for (const auto& v : wb)
            {
                if ((c1 >= c0) && (v[i] == 1)) nwb.push_back(v);
                if ((c1 < c0) && (v[i] == 0)) nwb.push_back(v);
            }
            wb = nwb;
            i++;
        }

        value = 0;
        mult = 1;
        for (int i = 11; i >= 0; i--)
        {
            if (wb[0][i] == 1)
            {
                value += mult;
            }
            mult *= 2;
        }

        wb = bits;
        i = 0;
        while (wb.size() != 1)
        {
            size_t c1 = 0;
            size_t c0 = 0;
            for (const auto& v : wb)
            {
                if (v[i] == 1) c1++;
                else c0++;
            }

            std::vector<std::vector<unsigned>> nwb;

            for (const auto& v : wb)
            {
                if ((c0 <= c1) && (v[i] == 0)) nwb.push_back(v);
                if ((c0 > c1) && (v[i] == 1)) nwb.push_back(v);
            }
            wb = nwb;
            i++;

        }

        long value2 = 0;
        mult = 1;
        for (int i = 11; i >= 0; i--)
        {
            if (wb[0][i] == 1)
            {
                value2 += mult;
            }
            mult *= 2;
        }
        set_star2_result(value * value2);
    }
};
