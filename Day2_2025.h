#pragma once
#include "helper.h"

#include <cmath>

size_t intPow10(size_t e) 
{
    size_t result = 1;
    while (e-- > 0) {
        result *= 10;
    }
    return result;
}

std::vector<size_t> same_halves_in_range(size_t L, size_t R) 
{
    std::vector<size_t> result;
    if (R < 0) return result;
    if (L < 0) L = 0;

    size_t maxDigit = static_cast<size_t>(std::to_string(R).size());

    // Only even lengths: 2, 4, 6, ...
    for (size_t len = 2; len <= maxDigit; len += 2) 
    {
        size_t halfLen = len / 2;

        size_t start = intPow10(halfLen - 1);        // no leading zero
        size_t end = intPow10(halfLen) - 1;

        size_t base = intPow10(halfLen);             // 10^halfLen

        for (size_t half = start; half <= end; ++half) 
        {
            size_t num = half * base + half;         // concatenate half || half

            if (num > R) break;                         // further 'half' only increases num
            if (num >= L) result.push_back(num);
        }
    }

    // Ensure sorted & unique, though it should already be
    std::sort(result.begin(), result.end());
    result.erase(unique(result.begin(), result.end()), result.end());
    return result;
}


std::vector<size_t> repeated_block_numbers_in_range(size_t L, size_t R) 
{
    std::vector<size_t> result;
    if (R < 0) return result;
    if (L < 0) L = 0;

    int maxDigits = static_cast<int>(std::to_string(R).size());

    // block length k: from 1 up to maxDigits / 2
    for (size_t k = 1; k <= maxDigits / 2; ++k) 
    {
        size_t pow10k = intPow10(k);

        // repetition count m: at least 2, up to maxDigits / k
        for (size_t m = 2; m <= maxDigits / k; ++m) 
        {
            size_t len = k * m;
            if (len > maxDigits) break;

            // coef = 1 + 10^k + 10^(2k) + ... + 10^(k*(m-1))
            size_t coef = 0;
            size_t term = 1;
            for (int i = 0; i < m; ++i) 
            {
                coef += term;
                term *= pow10k;
            }

            if (coef == 0) continue; // safety

            size_t blockStart = intPow10(k - 1);         // no leading zero
            size_t blockEnd;

            // Limit blockEnd so that block * coef <= R
            if (coef > 0) 
            {
                blockEnd = (intPow10(k) - 1) < (R / coef) ? (intPow10(k) - 1) : R / coef;
                
            }
            else {
                continue;
            }

            if (blockStart > blockEnd) 
            {
                continue; // no valid blocks for this (k, m)
            }

            for (size_t block = blockStart; block <= blockEnd; ++block) 
            {
                size_t num = block * coef;
                if (num < L) continue;   // too small
                if (num > R) break;      // further blocks only increase num
                result.push_back(num);
            }
        }
    }

    sort(result.begin(), result.end());
    result.erase(unique(result.begin(), result.end()), result.end());
    return result;
}

template<>
class day<2, 2025> : public day_base<2,2025>
{
    inline bool is_test() const override { return false; }
    void run_internal() override
    {
        size_t count_1 = 0;
        size_t count_2 = 0;
        while (!input_reader().is_end_of_file())
        {
            auto ranges = utility::string::split(input_reader().get_line(), ',');
            for (const auto& r : ranges)
            {
                auto numbers = utility::string::split_convert_vector<size_t>(r, '-', utility::string::stoull());
                auto p = same_halves_in_range(numbers[0], numbers[1]);
                for (const auto v : p)
                {
                    count_1 += v;
                }

                auto s = repeated_block_numbers_in_range(numbers[0], numbers[1]);
                for (const auto v : s)
                {
                    count_2 += v;
                }

            }
        }
        set_star1_result(count_1);
        set_star2_result(count_2);
    }
};