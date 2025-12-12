#pragma once
#include "helper.h"
using namespace utility::arrays;
using namespace utility::string;
using namespace utility::math;


template<>
class day<12, 2025> : public day_base<12, 2025>
{
    inline bool is_test() const override { return false; }

    void run_internal() override
    {
        std::vector<size_t> points;

        while (points.size() < 6)
        {
            size_t p = 0;
            auto id = input_reader().get_line();
            for (size_t i = 0; i < 3; i++)
            {
                for (const auto c : input_reader().get_line()) if (c == '#') p++;
            }
            auto empty = input_reader().get_line();
            points.push_back(p);
        }

        size_t s1 = 0;

        input_reader().read_by_line_until_condition_met_or_eof<size_t>(file_reader::no_condition(), [&](const auto& line, const auto)
            {
                auto split1 = split(line, ':');
                auto dims = split_and_convert(split1[0], 'x', stoull());
                auto counts = split_and_convert(split1[1], ' ', stoull());

                size_t required_space = 0;
                for (size_t i = 0; i < counts.size(); i++) required_space += counts[i] * points[i];
                if (required_space <= dims[0] * dims[1]) s1++;
            });
        
        set_star1_result(s1);
        set_star2_result(1);
    }
};