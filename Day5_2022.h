#pragma once
#include "helper.h"

template<>
class day<5, 2022> : public day_base<5,2022>
{
        void parse(const std::string& t, std::vector<std::list<char>>& output)
        {
            size_t i = 0;
            while (i < t.size())
            {
                if (output.size() < ((i / 4) + 1))
                {
                    output.push_back({});
                }
                if (t[i + 1] != ' ')
                {
                    output[i / 4].push_back(t[i + 1]);
                }
                i += 4;
            }
        }

        std::string take_top_items(const std::vector<std::list<char>>& positions)
        {
            std::string result(positions.size(),' ');
            std::transform(std::begin(positions), std::end(positions), std::begin(result), [](const auto item) { return item.front(); });
            return result;
        }

        void run_internal() override
        {
            std::vector<std::list<char>> positions_star1;

            auto line = input_reader().get_line();
            while (contains(line, '['))
            {
                parse(line, positions_star1);
                line = input_reader().get_line();
            }
            auto positions_star2 = positions_star1;
            
            line = input_reader().get_line();

            while (!input_reader().is_end_of_file())
            {
                auto line = input_reader().get_line();
                if (line.size() > 0)
                {
                    int count, from, to;
                    sscanf_s(line.data(), "move %d from %d to %d", &count, &from, &to);
                    from--;
                    to--;

                    for (size_t i = 0; i < count; i++)
                    {
                        positions_star1[to].push_front(positions_star1[from].front());
                        positions_star1[from].pop_front();
                    }

                    std::list<char> tmp_array;

                    for (size_t i = 0; i < count; i++)
                    {
                        tmp_array.push_front(positions_star2[from].front());
                        positions_star2[from].pop_front();
                    }

                    for (auto c : tmp_array)
                    {
                        positions_star2[to].push_front(c);
                    }
                }
            }

            set_star1_result(take_top_items(positions_star1));
            set_star2_result(take_top_items(positions_star2));
        }
};