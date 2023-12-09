#pragma once
#include "helper.h"

template<>
class day<8, 2023> : public day_base<8,2023>
{
        void run_internal() override
        {
            std::map < size_t, std::pair< size_t, size_t>> map;
            std::map<std::string, std::size_t> names_to_ids;
            
            std::string directions = input_reader().get_line();
            size_t id = 0;

            input_reader().skip_line();

            while (!input_reader().is_end_of_file())
            {
                auto in = helper::extract_from_input_according_to_positions(input_reader().get_line(), { {0,2},{7,9},{12,14} });
                if (!names_to_ids.contains(in[0])) names_to_ids[in[0]] = id++;
                if (!names_to_ids.contains(in[1])) names_to_ids[in[1]] = id++;
                if (!names_to_ids.contains(in[2])) names_to_ids[in[2]] = id++;
                map[names_to_ids.at(in[0])] = { names_to_ids.at(in[1]), names_to_ids.at(in[2]) };
            }

            auto possible_starts = helper::extract_if<size_t>(names_to_ids, [](const auto& i) { return i.first[2] == 'A'; }, [](const auto& i) {return i.second; });
            auto possible_ends = helper::extract_if<size_t>(names_to_ids, [](const auto& i) { return i.first[2] == 'Z'; }, [](const auto& i) {return i.second; });

            std::vector<size_t> act = helper::set_to_vector(possible_starts);
            std::vector<size_t> results;

            for (auto& a : act)
            {
                size_t steps = 0;
                while (true)
                {
                    if (possible_ends.contains(a)) break;
                    auto direction = directions[steps % directions.size()];
                    steps++;

                    if (direction == 'L')
                    {
                        a = map[a].first;
                    }
                    else if (direction == 'R')
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