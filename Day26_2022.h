#pragma once
#include "helper.h"

template<>
class day<26, 2022> : public day_base<26, 2022>
{   
    void print_space(const std::map<std::pair<size_t, size_t>, char>& s)
    {
        min_max_counter<long> x_limits;
        min_max_counter<long> y_limits;
        for (const auto& i : s)
        {
            x_limits.check_value(static_cast<long>(i.first.first));
            y_limits.check_value(static_cast<long>(i.first.second));
        }

        for (long y = y_limits.minimum(); y <= y_limits.maximum(); y++)
        {
            for (long x = x_limits.minimum(); x <= x_limits.maximum(); x++)
            {
                if (s.contains({ x,y })) std::cout << s.at({ x,y });
                else std::cout << ' ';
            }
            std::cout << std::endl;
        }
    }

    void run_internal() override
    {
        std::map<std::pair<size_t, size_t>, char> result;
        std::string in;
        while (!input_reader().is_end_of_file())
        {
            in = input_reader().get_line();
        }

        for (size_t i = 0; i < in.size(); i++)
        {
            long x = 0;
            long y = 0;
            bool settled = false;
            while (!settled)
            {
                if (y == 20)
                {
                    result[{x, y}] = in[i];
                    settled = true;
                }
                else
                {
                    if (!result.contains({ x,y + 1 }))
                    {
                        y += 1;
                    }
                    else if (!result.contains({ x - 1,y + 1 }))
                    {
                        x -= 1; y += 1;
                    }
                    else if (!result.contains({ x + 1,y + 1 }))
                    {
                        x += 1; y += 1;
                    }
                    else
                    {
                        result[{x, y}] = in[i];
                        settled = true;
                    }
                }
            }
            print_space(result);
            std::cout << std::endl;
        }
    }


/*
    void run_internal() override
    {
        std::map<std::pair<size_t, size_t>, char> orig;
        size_t y = 0;
        while (!input_reader().is_end_of_file())
        {
            
            auto l = input_reader().get_line();
            for (size_t i = 0; i < l.size(); i++)
            {
                if (l[i] != ' ')
                {
                    orig[{i, y}] = l[i];
                }
            }
            y++;
        }

        print_space(orig);
        std::cout << std::endl;

        std::map<std::pair<size_t, size_t>, char> space1;
        std::map < size_t, std::pair<size_t, size_t>> mapping;
        
        for (size_t i = 0; i < orig.size(); i++)
        {
            long x = 17;
            long y = 0;
            bool settled = false;
            while (!settled)
            {
                if (space1.contains({ x,y }))
                {
                    throw;
                }

                if (y == 17)
                {
                    space1[{x, y}] = 'o';
                    mapping[i] = {x,y};
                    settled = true;
                }
                else
                {
                    if (!space1.contains({ x,y + 1 }))
                    {
                        y += 1;
                    }
                    else if (!space1.contains({ x - 1,y + 1 }))
                    {
                        x -= 1; y += 1;
                    }
                    else if (!space1.contains({ x + 1,y + 1 }))
                    {
                        x += 1; y += 1;
                    }
                    else
                    {
                        space1[{x, y}] = 'o';
                        mapping[i] = { x,y };
                        settled = true;
                    }
                }
            }
            //std::cout << std::endl;
        }

        //print_space(space1);
        std::cout << std::endl;

        std::ofstream file;
        file.open("d:\\intput.txt");
        for (const auto& m : mapping)
        {
            file << orig.at(m.second);
            std::cout << orig.at(m.second);
        }
        file.close();
    }
*/
};

