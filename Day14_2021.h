#pragma once
#include "helper.h"

using namespace utility::arrays;
using namespace utility::geometry;
using namespace utility::math;
using namespace utility::string;


template<>
class day<14, 2021> : public day_base<14, 2021>
{
    size_t calculate_result(const std::map<std::pair<char, char>, size_t>& in, const char last)
    {
        std::map<char, size_t> amount = { {last, 1} };
        for (auto c : in)
        {
            modify_value_in_map_safe<char, size_t>(amount, c.first.first, 0, [c](const auto& val) {return val + c.second;});
        }

        min_max_counter<size_t> counter;
        for (auto m : amount)
        {
            counter.check_value(m.second);
        }
        
        return counter.maximum() - counter.minimum();
    };
    
    
    
    void run_internal() override
    {
        std::map<std::pair<char, char>, char> inserts;
        auto input = input_reader().get_line();
        input_reader().get_line();
        while (!input_reader().is_end_of_file())
        {
            auto line = input_reader().get_line();
            char c1 = line[0];
            char c2 = line[1];
            char c3 = line[6];
            inserts[{c1, c2}] = c3;
        }

        std::map<std::pair<char, char>, size_t> mapped_pairs;
        for (size_t i = 0; i < input.size() - 1; i++)
        {
            modify_value_in_map_safe<std::pair<char,char>, size_t>(mapped_pairs, { input[i], input[i + 1] }, 0, [](const auto& val) { return val+1; });
        }

        for (size_t step = 0; step < 40; step++)
        {
            std::map<std::pair<char, char>, size_t> local_mapped_pairs;
            for (auto& m : mapped_pairs)
            {
                auto f = inserts.find(m.first);
                if (f != inserts.end())
                {
                    modify_value_in_map_safe<std::pair<char, char>, size_t>(local_mapped_pairs, { m.first.first, f->second }, 0, [m](const auto& val) { return val + m.second; });
                    modify_value_in_map_safe<std::pair<char, char>, size_t>(local_mapped_pairs, { f->second, m.first.second }, 0, [m](const auto& val) {return val + m.second; });
                }
            }
            
            mapped_pairs = local_mapped_pairs;
            
            if (step == 9)
            {
                set_star1_result(calculate_result(mapped_pairs, input[input.size() - 1]));
            }
            
            if (step == 39)
            {
                set_star2_result(calculate_result(mapped_pairs, input[input.size() - 1]));
            }
        }
    }
};
