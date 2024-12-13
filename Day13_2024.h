#pragma once
#include "helper.h"

template<>
class day<13, 2024> : public day_base<13,2024>
{
    template <typename T = uint64_t>
    bool is_decimal_only(const double v)
    {
        return (v - static_cast<T>(v)) == 0;
    }
    
    uint64_t calculate_score(uint64_t A1, uint64_t B1, uint64_t C1, uint64_t A2, uint64_t B2, uint64_t C2)
    {
        auto res = utility::math::solve_2unknowns(static_cast<double>(A1), static_cast<double>(B1), static_cast<double>(C1), static_cast<double>(A2), static_cast<double>(B2), static_cast<double>(C2));
        if (res.has_result)
        {
            if (is_decimal_only(res.x) && is_decimal_only(res.y))
            {
                return static_cast<uint64_t>(res.x) * 3 + static_cast<uint64_t>(res.y);
            }
        }
        return 0;
    }


    inline bool is_test() const override { return false; }
    void run_internal() override
    {
        uint64_t sum1 = 0;
        uint64_t sum2 = 0;

        while (!input_reader().is_end_of_file())
        {
            long A1, A2, B1, B2, C1, C2;

            sscanf_s(input_reader().get_line().c_str(), "Button A : X + %ld, Y + %ld", &A1, &A2);
            sscanf_s(input_reader().get_line().c_str(), "Button B : X + %ld, Y + %ld", &B1, &B2);
            sscanf_s(input_reader().get_line().c_str(), "Prize : X = %ld, Y = %ld", &C1, &C2);
            input_reader().get_line();
            
            sum1 += calculate_score(A1, B1, C1, A2, B2, C2);
            sum2 += calculate_score(A1, B1, 10000000000000 + C1, A2, B2, 10000000000000 + C2);
        }

        set_star1_result(sum1);
        set_star2_result(sum2);
    }
};