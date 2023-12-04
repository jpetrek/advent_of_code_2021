#pragma once
#include "helper.h"

template<>
class day<4, 2023> : public day_base<4,2023>
{
    void run_internal() override
    {
        struct card
        {
            size_t number_of_hits;
            size_t number_of_cards;
        };
        
        std::vector<card> cards;

        while (!input_reader().is_end_of_file())
        {
            auto sets = helper::split(helper::split(input_reader().get_line(), ':')[1], '|');
            
            auto card_numbers = helper::split_and_convert<size_t>(sets[0], ' ', [](const auto& in) {return stoul(in); });
            auto hand_numbers = helper::split_and_convert<size_t>(sets[1], ' ', [](const auto& in) {return stoul(in); });

            std::map<size_t, size_t> solver;
            for (const auto cn : card_numbers) helper::modify_value_in_map_safe<size_t, size_t>(solver, cn, 0, [](const auto& v) { return v+1; });
            for (const auto hn : hand_numbers) helper::modify_value_in_map_safe<size_t, size_t>(solver, hn, 0, [](const auto& v) { return v+1; });

            cards.push_back({ helper::accumulate_if<size_t>(solver, 0, [](const auto& i) { return i.second > 1 ? 1 : 0; }), 1 });
        }

        for (size_t i = 0; i < cards.size(); i++)
        {
            for (size_t j = 0; j < cards[i].number_of_cards; j++)
            {
                for (size_t ci = 1; ci <= cards[i].number_of_hits; ci++)
                {
                    if (i + ci < cards.size()) cards[i + ci].number_of_cards++;
                }
            }
        }

        set_star1_result(helper::accumulate_if<size_t>(cards, 0, [](const auto& i) { return i.number_of_hits > 0 ? static_cast<size_t>(pow(2, i.number_of_hits - 1)) : 0; }));
        set_star2_result(helper::accumulate_if<size_t>(cards, 0, [](const auto& i) { return i.number_of_cards; }));
    }
};