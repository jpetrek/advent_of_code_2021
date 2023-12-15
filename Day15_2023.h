#pragma once
#include "helper.h"

template<>
class day<15, 2023> : public day_base<15, 2023>
{
    size_t calculate_hash(const std::string& s)
    {
        size_t ret = 0;
        for (auto c : s)
        {
            ret += c;
            ret *= 17;
            ret %= 256;
        }
        return ret;
    }
    
    struct box
    {
        std::map < std::string, size_t > name_position;
        std::vector<size_t> values;
    };
    
    void run_internal() override
    {
        std::vector<box> boxes(256, box());
        std::string bs;
        //set_is_test(true);
        while (!input_reader().is_end_of_file())
        {
            bs += input_reader().get_line();
        }
        auto subs = helper::split(bs, ',');
        size_t s1 = 0;
        for (const auto& s : subs)
        {
            s1 += calculate_hash(s);
            
            if (contains(s, '='))
            {
                auto parts = helper::split(s, '=');
                auto index = calculate_hash(parts[0]);
                size_t value = stoull(parts[1]);
                if (boxes[index].name_position.contains(parts[0])) boxes[index].values[boxes[index].name_position[parts[0]]] = value;
                else
                {
                    boxes[index].values.push_back(value);
                    boxes[index].name_position[parts[0]] = boxes[index].values.size() - 1;
                }
            }
            else
            {
                auto parts = helper::split(s, '-');
                auto index = calculate_hash(parts[0]);
                if (boxes[index].name_position.contains(parts[0]))
                {
                    size_t i = boxes[index].name_position[parts[0]];
                    for (auto& m : boxes[index].name_position) if (m.second > i) m.second--;
                    boxes[index].values.erase(std::begin(boxes[index].values) + boxes[index].name_position[parts[0]]);
                    boxes[index].name_position.erase(parts[0]);
                }
            }
        }

        set_star1_result(s1);
        size_t s2 = 0;
        for (size_t i =0; i< boxes.size(); i++)
        {
            size_t bn = 0;
            for (const auto& n : boxes[i].name_position)
            {
                bn += (i + 1) * (n.second + 1) * boxes[i].values[n.second];
            }
            s2 += bn;
        }

        set_star2_result(s2);
    }
};
