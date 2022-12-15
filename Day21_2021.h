#pragma once
#include "helper.h"

struct cache_key
{
    size_t p1;
    size_t p2;
    size_t s1;
    size_t s2;
};

bool operator < (const cache_key& k1, const cache_key& k2)
{
    if (k1.p1 != k2.p1)
    {
        return k1.p1 < k2.p1;
    }

    if (k1.p2 != k2.p2)
    {
        return k1.p2 < k2.p2;
    }

    if (k1.s1 != k2.s1)
    {
        return k1.s1 < k2.s1;
    }
    return k1.s2 < k2.s2;
}


template<>
class day<21, 2021> : public day_base<21, 2021>
{

    size_t get_dice_number(size_t &dice, size_t& rolls)
    {
        rolls++;
        dice++;
        if (dice == 101) dice = 1;
        return dice;
    }

    std::pair<size_t, size_t> calculate(size_t p1, size_t p2, const std::vector<size_t>& sums, std::map<cache_key, std::pair<size_t, size_t>>& cache, size_t s1, size_t s2)
    {
        cache_key k = { p1,p2,s1,s2 };
        auto it = cache.find(k);
        if (it != std::end(cache))
        {
            return it->second;
        }

        std::pair<size_t, size_t> ret{ 0,0 };
        for (auto r : sums)
        {
            auto new_p1 = p1 + r;
            while (new_p1 > 10) new_p1 -= 10;
            auto new_s1 = s1 + new_p1;
            if (new_s1 >= 21)
            {
                ret.first++;
            }
            else
            {
                auto d = calculate(p2, new_p1, sums, cache, s2, new_s1);
                ret.first += d.second;
                ret.second += d.first;
            }
        }
        cache[k] = ret;
        return ret;
    }

    void run_internal() override
    {
        size_t p1 = 8;
        size_t p2 = 6;

        size_t score_1 = 0;
        size_t score_2 = 0;
        size_t sp1 = p1;
        size_t sp2 = p2;
        size_t r = 0;
        size_t dice = 0;

        while ((score_1 < 1000) && (score_2 < 1000))
        {
            sp1 += get_dice_number(dice,r) + get_dice_number(dice, r) + get_dice_number(dice, r);
            while (sp1 > 10) sp1 -= 10;
            score_1 += sp1;
            if (score_1 < 1000)
            {
                sp2 += get_dice_number(dice, r) + get_dice_number(dice, r) + get_dice_number(dice, r);
                while (sp2 > 10) sp2 -= 10;
                score_2 += sp2;
            }
        }

        set_star1_result(score_1 < score_2 ? r * score_1 : r*score_2 );

        std::vector<size_t> possibilities;
        for (size_t i = 1; i < 4; i++)
            for (size_t j = 1; j < 4; j++)
                for (size_t k = 1; k < 4; k++)
                    possibilities.push_back(i + j + k);

        std::map<cache_key, std::pair<size_t, size_t>> cache;
        auto result = calculate(p1, p2, possibilities, cache, 0, 0);
        set_star2_result(result.first > result.second ? result.first : result.second);
    }
};
