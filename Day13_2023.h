#pragma once
#include "helper.h"

template<>
class day<13, 2023> : public day_base<13,2023>
{
    size_t find_mirroring_line(const std::vector<std::vector<char>>& pattern, const size_t star)
    {
        for (size_t y_split = 0; y_split < pattern.size() - 1; y_split++)
        {
            std::vector<size_t> mishits;
            for (size_t y = 0; y < std::min(y_split + 1, pattern.size() - (y_split + 1)); y++)
            {
                for (size_t x = 0; x < pattern[0].size(); x++)
                {
                    if ((pattern[y_split - y][x] != pattern[y_split + 1 + y][x]) && mishits.size() < 2) mishits.push_back(y_split + 1);
                }
            }

            if (mishits.size() == 1 && star == 2) return mishits[0];
            if (mishits.size() == 0 && star == 1) return y_split + 1;
        }

        return 0;
    }

    void run_internal() override
    {
        std::vector<std::vector<std::vector<char>>> patterns;
        while (!input_reader().is_end_of_file())
        {
            patterns.push_back(utility::io::transform_input_into_array<char>(input_reader(), [](char c) { return c; }));
        }

        size_t  s1 = 0;
        size_t  s2 = 0;
        for (const auto& p : patterns)
        {
            const auto transposed = utility::arrays::transpose(p);
            s1 += 100 * find_mirroring_line(p, 1) + find_mirroring_line(transposed, 1);
            s2 += 100 * find_mirroring_line(p, 2) + find_mirroring_line(transposed, 2);
        }
        set_star1_result(s1);
        set_star2_result(s2);
    }
};