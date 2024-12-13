#pragma once
#include "helper.h"

using namespace utility::math;

template<>
class day<13, 2024> : public day_base<13,2024>
{
    uint64_t calculate_score(const uint64_t A1, const uint64_t B1, const uint64_t C1, const  uint64_t A2, const uint64_t B2, const uint64_t C2, const uint64_t limit) const
    {
        auto res =solve_2unknowns(static_cast<double>(A1), static_cast<double>(B1), static_cast<double>(C1), static_cast<double>(A2), static_cast<double>(B2), static_cast<double>(C2));
        if ((res.size() == 2) && is_decimal_only(res[0]) && is_decimal_only(res[1]) && (static_cast<uint64_t>(res[0])<= limit) && (static_cast<uint64_t>(res[1]) <= limit))
        {
            return static_cast<uint64_t>(res[0]) * 3 + static_cast<uint64_t>(res[1]);
        }
        return 0;
    }

    inline bool is_test() const override { return false; }
    
    void run_internal() override
    {
        const uint64_t second_star_increment = 10000000000000;
        uint64_t sum1 = 0;
        uint64_t sum2 = 0;

        while (!input_reader().is_end_of_file())
        {
            long A1, A2, B1, B2, C1, C2;

            sscanf_s(input_reader().get_line().c_str(), "Button A : X + %ld, Y + %ld", &A1, &A2);
            sscanf_s(input_reader().get_line().c_str(), "Button B : X + %ld, Y + %ld", &B1, &B2);
            sscanf_s(input_reader().get_line().c_str(), "Prize : X = %ld, Y = %ld", &C1, &C2);
            input_reader().get_line();
            
            sum1 += calculate_score(A1, B1, C1, A2, B2, C2, 100);
            sum2 += calculate_score(A1, B1, C1 + second_star_increment , A2, B2, C2 + second_star_increment, std::numeric_limits<uint64_t>::max());
        }

        set_star1_result(sum1);
        set_star2_result(sum2);
    }
};