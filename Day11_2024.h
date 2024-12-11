#pragma once
#include "helper.h"
using namespace utility::arrays;
using namespace utility::string;
using namespace utility::math;


template<>
class day<11, 2024> : public day_base<11,2024>
{
    typedef std::map<uint64_t, size_t> stones_count;

    inline bool is_test() const override { return false; }

    void add_stones(stones_count& stones, uint64_t stone_id, size_t count) const
    {
        modify_map(stones, stone_id, 0, [count](auto& m) { m += count; });
    }

    size_t count_stones(stones_count stones, const size_t number_of_blinks) const
    {
        for (size_t i = 0; i < number_of_blinks; i++)
        {
            stones_count new_stones;
            for (const auto s : stones)
            {
                if (s.first == 0)
                {
                    add_stones(new_stones, 1, s.second);
                }
                else
                {
                    auto nd = digits10<uint64_t>(s.first);
                    if (nd % 2 == 0)
                    {
                        auto top = pow10(nd / 2);
                        add_stones(new_stones, s.first / top, s.second);
                        add_stones(new_stones, s.first % top, s.second);
                    }
                    else
                    {
                        add_stones(new_stones, s.first * 2024, s.second);
                    }
                }
            }
            stones = new_stones;
        }

        return accumulate_generic<size_t>(stones, 0, [](auto s, const auto stone) {return s + stone.second; });
    }

    void run_internal() override
    {
        auto stones = split_and_convert_generic<stones_count>(input_reader().get_line(), ' ', [](auto& stones, auto s){ modify_map(stones, stoull(s), 0, [](auto& c) {c++; }); });
        set_star1_result(count_stones(stones,25));
        set_star2_result(count_stones(stones, 75));
    }
};