#pragma once
#include "helper.h"

using namespace utility::arrays;
using namespace utility::geometry;
using namespace utility::math;
using namespace utility::string;

template<>
class day<4, 2023> : public day_base<4,2023>
{
    void run_internal() override
    {
        struct card
        {
            size_t hits;
            size_t count;
        };
        
        std::vector<card> cards;

        while (!input_reader().is_end_of_file())
        {
            auto sets = split(split(input_reader().get_line(), ':')[1], '|');
            auto card_numbers = split_convert_set<size_t>(sets[0], ' ', [](const auto& in) { return stoul(in); });
            auto hand_numbers = split_convert_set<size_t>(sets[1], ' ', [](const auto& in) { return stoul(in); });
            cards.push_back({ intersect(card_numbers, hand_numbers).size(), 1 });
        }

        for (size_t i = 0; i < cards.size(); i++)
        {
            for (size_t j = 0; j < cards[i].count; j++)
            {
                for (size_t ci = 1; ci <= cards[i].hits; ci++)
                {
                    if (i + ci < cards.size()) cards[i + ci].count++;
                }
            }
        }

        set_star1_result(accumulate_if<size_t>(cards, 0, [](auto index, const auto& i) { return i.hits > 0 ? static_cast<size_t>(pow(2, i.hits - 1)) : 0; }));
        set_star2_result(accumulate_if<size_t>(cards, 0, [](auto index, const auto& i) { return i.count; }));
    }
};