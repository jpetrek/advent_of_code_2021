#pragma once
#include "helper.h"
template<>
class day<7, 2023> : public day_base<7,2023>
{
        const std::map<char, size_t> ordering = { {'2', 0}, {'3', 1}, {'4', 2}, {'5', 3}, {'6', 4}, {'7', 5}, {'8', 6}, {'9', 7}, {'T', 8}, {'J', 9}, {'Q', 10}, {'K', 11}, {'A', 12} };
        const std::map<char, size_t> ordering_joker = { {'J', 0}, {'2', 1}, {'3', 2}, {'4', 3}, {'5', 4}, {'6', 5}, {'7', 6}, {'8', 7}, {'9', 8}, {'T', 9}, {'Q', 10}, {'K', 11}, {'A', 12} };
    
        enum hand_scores
        {
            five = 7,
            four = 6,
            full_house = 5,
            three = 4,
            two_pairs = 3,
            pair = 2,
            one = 1
        };
    
        size_t first_rule_score(const std::string& card)
        {
            std::map<char, size_t> score;
            for (const auto c : card)
            {
                helper::modify_value_in_map_safe<char, size_t>(score, c, 0, [](const auto& i) {return i+1; });
            }

            size_t five = 0;
            size_t four = 0;
            size_t three = 0;
            size_t two = 0;
            for (const auto& s : score)
            {
                if (s.second == 5) five++;
                if (s.second == 4) four++;
                if (s.second == 3) three++;
                if (s.second == 2) two++;
            }

            if (five == 1) return hand_scores::five;
            if (four == 1) return hand_scores::four;
            if ((three == 1) && (two == 1)) return hand_scores::full_house;
            if ((three == 1) && (two == 0)) return hand_scores::three;
            if (two == 2) return hand_scores::two_pairs;
            if (two == 1) return hand_scores::pair;
            return hand_scores::one;    
        }

        size_t first_rule_joker_score(const std::string& card)
        {
            size_t Js = 0;
            std::map<char, size_t> score;
            for (const auto c : card)
            {
                if (c == 'J')
                {
                    Js++;
                }
                else
                {
                    helper::modify_value_in_map_safe<char, size_t>(score, c, 0, [](const auto& i) {return i + 1; });
                }
            }

            size_t five = 0;
            size_t four = 0;
            size_t three = 0;
            size_t two = 0;
            
            for (const auto& s : score)
            {
                if (s.second == 5) five++;
                if (s.second == 4) four++;
                if (s.second == 3) three++;
                if (s.second == 2) two++;
            }

            if (Js == 0)
            {
                if (five == 1) return hand_scores::five;
                if (four == 1) return hand_scores::four;
                if ((three == 1) && (two == 1)) return hand_scores::full_house;
                if ((three == 1) && (two == 0)) return hand_scores::three;
                if (two == 2) return hand_scores::two_pairs;
                if (two == 1) return hand_scores::pair;
                return hand_scores::one;
            }
            
            if (Js == 1)
            {
                if (four == 1) return hand_scores::five;
                if (three == 1) return hand_scores::four;
                if (two == 2) return hand_scores::full_house;
                if (two == 1) return hand_scores::three;
                return hand_scores::pair;
            }

            if (Js == 2)
            {
                if (three == 1) return hand_scores::five;
                if (two == 1) return hand_scores::four;
                return hand_scores::three;
            }

            if (Js == 3)
            {
                if (two == 1) return hand_scores::five;
                return hand_scores::four;
            }

            if (Js == 4)
            {
                return hand_scores::five;
            }

            if (Js == 5)
            {
                return hand_scores::five;
            }

            return hand_scores::one;
        }
    
        bool second_rule_less(const std::map<char, size_t>& order, const std::string& c1, const std::string c2)
        {
            size_t i = 0;
            while (order.at(c1[i]) == order.at(c2[i])) i++;
            return order.at(c1[i]) < order.at(c2[i]);
        }

        void run_internal() override
        {
            std::vector<std::pair<std::string, size_t>> cards;

            while (!input_reader().is_end_of_file())
            {
                auto l = helper::split(input_reader().get_line(), ' ');
                cards.push_back({ l[0], stoull(l[1]) });
            }
            
            std::sort(cards.begin(), cards.end(), [&](const auto& a, const auto& b)
                {
                    auto sa = first_rule_score(a.first);
                    auto sb = first_rule_score(b.first);
                    if (sa == sb)
                    {
                        return second_rule_less(ordering, a.first, b.first);
                    }
                    return sa < sb;
                });
            set_star1_result(helper::accumulate_if<size_t>(cards, 0, [](auto i, const auto& item) {return item.second * (i + 1); }));

            std::sort(cards.begin(), cards.end(), [&](const auto& a, const auto& b)
                {
                    auto sa = first_rule_joker_score(a.first);
                    auto sb = first_rule_joker_score(b.first);
                    if (sa == sb)
                    {
                        return second_rule_less(ordering_joker, a.first, b.first);
                    }
                    return sa < sb;
                });
            set_star2_result(helper::accumulate_if<size_t>(cards, 0, [](auto i, const auto& item) {return item.second * (i + 1); }));
        }
};