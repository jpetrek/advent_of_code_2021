#pragma once
#include "helper.h"

template<>
class day<3, 2024> : public day_base<3,2024>
{
    inline bool is_test() const override { return false; }
    
    std::vector<long> find_all_positions(const std::string& where, const std::string& what) const
    {
        std::vector<long> ret;
        size_t i = 0;
        while (i < where.size())
        {
            auto pos = where.find(what, i);
            if (pos != std::string::npos)
            {
                i = pos + what.size();
                ret.push_back(static_cast<long>(i));
            }
            else break;
        }
        return ret;
    }

    long parse_and_calculate_mul(const std::string& where, size_t pos) const
    {
        size_t n1_s = pos, n1_e = pos;
        while ((n1_e < where.size()) && (isdigit(where[n1_e])))  n1_e++;
        if (((n1_e - n1_s > 0) && (n1_e - n1_s < 4)) && ((n1_e < where.size()) && (where[n1_e] == ',')))
        {
            size_t n2_s = n1_e + 1, n2_e = n1_e + 1;
            while ((n2_e < where.size()) && (isdigit(where[n2_e]))) n2_e++;
            if ((n2_e - n2_s > 0) && (n2_e - n2_s < 4) && (n2_e < where.size()) && (where[n2_e] == ')')) return stol(where.substr(n1_s, n1_e - n1_s)) * stol(where.substr(n2_s, n2_e - n2_s));
        }
        return 0;
    }

    void run_internal() override
    {
        long sum_m = 0, sum_m2 = 0;
        auto s = input_reader().read_file();
        auto mul_possible_starts = find_all_positions(s, "mul(");
        auto do_possitions = find_all_positions(s, "do()");
        auto dont_possitions = find_all_positions(s, "don't()");

        for (auto p : mul_possible_starts)
        {
            auto v = parse_and_calculate_mul(s, p);
            sum_m += v;
            if (closest(do_possitions, p) >= closest(dont_possitions, p)) sum_m2 += v;
        }

        set_star1_result(sum_m);
        set_star2_result(sum_m2);
    }
};