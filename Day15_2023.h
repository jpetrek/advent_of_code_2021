#pragma once
#include "helper.h"

template<>
class day<15, 2023> : public day_base<15, 2023>
{
    struct box
    {
        std::map < std::string, size_t > name_position;
        std::vector<size_t> values;
    };
    
    void run_internal() override
    {
        const std::function<size_t(const std::string&)> hash = [](const std::string& s) { return helper::accumulate_generic<size_t>(s, 0, [](const auto r, const auto& c) { return ((r + c) * 17) % 256; }); };
        std::vector<box> boxes(256, box());
        //set_is_test(true);

        auto commands = helper::split(input_reader().read_file(), ',');

        size_t s1 = 0;
        for (const auto& command : commands)
        {
            s1 += hash(command);
            if (contains(command, '='))
            {
                auto parts = helper::split(command, '=');
                auto index = hash(parts[0]);
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
                auto parts = helper::split(command, '-');
                auto index = hash(parts[0]);
                if (boxes[index].name_position.contains(parts[0]))
                {
                    size_t i = boxes[index].name_position[parts[0]];
                    for (auto& m : boxes[index].name_position) m.second += (m.second > i) ? -1 : 0;
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
