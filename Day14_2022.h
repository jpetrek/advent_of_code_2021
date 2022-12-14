#pragma once
#include "helper.h"

template<>
class day<14, 2022> : public day_base<14, 2022>
{
    enum class part
    {
        rock,
        ball
    };

    void print_space(const std::map<std::pair<int, int>, part>& space)
    {
        min_max_counter<int> limits_x;
        min_max_counter<int> limits_y;
        for (const auto& s : space)
        {
            limits_x.check_value(s.first.first);
            limits_y.check_value(s.first.second);
        }

        for (int y = limits_y.minimum(); y <= limits_y.maximum(); y++)
        {
            for (int x = limits_x.minimum(); x <= limits_x.maximum(); x++)
            {
                if (space.contains({ x,y }))
                {
                    if (space.at({ x,y }) == part::rock) std::cout << '#';
                    else std::cout << 'o';
                }
                else
                {
                    std::cout << '.';
                }
            }
            std::cout << std::endl;
        }
    }

    void fill_line_in_space_by_rocks(std::pair<int, int> s, std::pair<int, int> e, std::map<std::pair<int, int>, part>& space)
    {
        if (s.first != e.first)
        {
            auto x_min_max = helper::get_min_max(s.first, e.first);
            for (int x = x_min_max.first; x <= x_min_max.second; x++)
            {
                space[{ x, s.second}] = part::rock;
            }
        }
        else 
        {
            auto y_min_max = helper::get_min_max(s.second, e.second);
            for (int y = y_min_max.first; y <= y_min_max.second; y++)
            {
                space[{ s.first, y}] = part::rock;
            }
        }
    }

    bool drop_ball_1(int bx, int by, std::map<std::pair<int, int>, part>& space, int void_y)
    {
        bool can_move = true;
        int x = bx;
        int y = by;
        while (can_move)
        {
            if (!space.contains({ x,y + 1 }))
            {
                y++;
            }
            else if (!space.contains({ x - 1 ,y + 1 }))
            {
                x--;
                y++;
            }
            else if (!space.contains({ x + 1 ,y + 1 }))
            {
                x++;
                y++;
            }
            else
            {
                can_move = false;
            }

            if (y > void_y) return false;
        }

        space[{x, y}] = part::ball;
        return true;
    }

    bool drop_ball_2(int bx, int by, std::map<std::pair<int, int>, part>& space, int void_y)
    {
        bool can_move = true;
        int x = bx;
        int y = by;
        while (can_move)
        {
            if (!space.contains({ x,y + 1 }))
            {
                y++;
            }
            else if (!space.contains({ x - 1 ,y + 1 }))
            {
                x--;
                y++;
            }
            else if (!space.contains({ x + 1 ,y + 1 }))
            {
                x++;
                y++;
            }
            else
            {
                can_move = false;
            }

            if (y == void_y + 1) can_move = false;
        }
        
        if ((x == bx) && (y == by)) return false;
        
        space[{x, y}] = part::ball;
        return true;
    }


    void run_interal() override
    {
        std::map<std::pair<int, int>, part> space_1;
        min_max_counter<int> limits_y;
        while (!input_reader().is_end_of_file())
        {
            auto part = helper::split(input_reader().get_line(), ' ');
            auto start = helper::vector_to_pair<int>(helper::split_and_convert<int>(part[0].data(), ','));
            size_t i = 1;
            while (i < part.size())
            {
                i++;
                auto end = helper::vector_to_pair<int>(helper::split_and_convert<int>(part[i].data(), ','));
                limits_y.check_value(start.second);
                limits_y.check_value(end.second);
                fill_line_in_space_by_rocks(start, end, space_1);
                start = end;
                i++;
            }
        }

        auto space_2 = space_1;
        size_t c = 0;
        while (drop_ball_1(500, 0, space_1, limits_y.maximum()))
        {
            c++;
        }
        set_star1_result(c);
        
        c = 0;
        while (drop_ball_2(500, 0, space_2, limits_y.maximum()))
        {
            c++;
        }
        set_star2_result(c+1);
        
        // print_space(space2); // just for fun :-D
    }
};
