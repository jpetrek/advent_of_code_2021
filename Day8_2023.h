#pragma once
#include "helper.h"

template<>
class day<8, 2023> : public day_base<8,2023>
{
        void run_internal() override
        {
            std::set<size_t> possible_starts;
            std::set<size_t> possible_ends;
            std::map<std::string, std::size_t> names_to_ids;
            std::map < size_t, std::pair< size_t, size_t>> map;
            
            std::string LR = input_reader().get_line();
            size_t id = 0;

            std::function<void(const std::string&)> mofify_map_propely = [&](const auto& s)
                {
                    if (names_to_ids.find(s) == std::end(names_to_ids))
                    {
                        names_to_ids[s] = id++;
                    }
                    if (s[2] == 'A')
                    {
                        possible_starts.insert(names_to_ids.at(s));
                    }
                    if (s[2] == 'Z')
                    {
                        possible_ends.insert(names_to_ids.at(s));
                    }
                };
            
            input_reader().get_line();
            while (!input_reader().is_end_of_file())
            {
                auto in = helper::extract_from_input_according_to_positions(input_reader().get_line(), { {0,2},{7,9},{12,14} });
                mofify_map_propely(in[0]);
                mofify_map_propely(in[1]);
                mofify_map_propely(in[2]);
                map[names_to_ids.at(in[0])] = { names_to_ids.at(in[1]), names_to_ids.at(in[2]) };
            }

            std::vector<size_t> act = helper::set_to_vector(possible_starts);
            std::vector<size_t> results;

            for (auto& a : act)
            {
                size_t steps = 0;
                while (true)
                {
                    if (possible_ends.contains(a)) break;
                    auto c = LR[steps % LR.size()];
                    steps++;

                    if (c == 'L')
                    {
                        a = map[a].first;
                    }
                    if (c == 'R')
                    {
                        a = map[a].second;
                    }
                }
                results.push_back(steps);
            }

            set_star1_result(results[0]);
            set_star2_result(helper::lcm(results));
        }
}; 