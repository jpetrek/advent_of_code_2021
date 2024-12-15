#pragma once
#include "helper.h"

template<>
class day<15, 2024> : public day_base<15,2024>
{
    typedef std::vector<direction_2d::name> directions;
    typedef point_2d_generic<long> position;
    typedef std::map<position, char> storage;

    inline bool is_test() const override { return false; }
    
    std::tuple<storage, long, long, position> make_input_twice_wide(const storage& space, const long max_x, const long max_y) const
    {
        storage space_2;
        position start_2;
        for (long y = 0; y < max_y; y++)
        {
            for (long x = 0; x < max_x; x++)
            {
                if ((space.at({x, y}) == '.') || (space.at({x, y}) == '#'))
                {
                    space_2[{x * 2, y}] = space.at({x, y});
                    space_2[{x * 2 + 1, y}] = space.at({x, y});
                }
                else if (space.at({x, y}) == '@')
                {
                    space_2[{x * 2, y}] = '@';
                    space_2[{x * 2 + 1, y}] = '.';
                    start_2 = { x * 2, y };
                }
                else
                {
                    space_2[{x * 2, y}] = '[';
                    space_2[{x * 2 + 1, y}] = ']';
                }
            }
        }
        return {space_2, max_x * 2, max_y, start_2};
    }

    void traverse_crates_1(const position& start, const direction_2d::name move, std::set<std::pair<position, char>>& movables, const storage& space) const
    {
        auto new_position = start;
        while ((space.at(new_position) != '#') && (space.at(new_position) != '.'))
        {
            movables.insert({ new_position, space.at(new_position) });
            new_position = add(new_position, move);
        }
    }

    void traverse_crates_2(const position& start, const direction_2d::name move, std::set<std::pair<position, char>>& movables, const storage& space) const
    {
        movables.insert({ start, space.at(start) });
        auto new_position = add(start, move);
        if (space.at(new_position) == '[' || space.at(new_position) == ']')
        {
            if ((move == direction_2d::up) || (move == direction_2d::down))
            {
                traverse_crates_2(new_position, move, movables, space);

                new_position.x += space.at(new_position) == '[' ? 1 : -1;
                traverse_crates_2(new_position, move, movables, space);
            }
            else
            {
                traverse_crates_2(new_position, move, movables, space);
            }
        }
    }

    storage solve(const storage& input, const directions& moves, const position& start, const size_t star) const
    {
        auto space = input;
        auto actual = start;
        for (auto m : moves)
        {
            std::set<std::pair<position, char>> possible_movables;
            if (star == 1) traverse_crates_1(actual, m, possible_movables, space);
            else traverse_crates_2(actual, m, possible_movables, space);
            bool move_ok = true;
            for (const auto& pm : possible_movables)
            {
                if (space.at(add(pm.first, m)) == '#')
                {
                    move_ok = false;
                    break;
                }
            }
            if (move_ok)
            {
                for (const auto& pm : possible_movables) space[pm.first] = '.';
                for (const auto& pm : possible_movables) space[add(pm.first, m)] = pm.second;
                actual = add(actual, m);
            }
        }
        
        return space;
    }

    uint64_t calculate_score(const storage& space, const long max_x, const long max_y, const char id) const
    {
        uint64_t sum = 0;
        for (long y = 0; y < max_y; y++)
        {
            for (long x = 0; x < max_x; x++)
            {
                if (space.at({x, y}) == id) sum += 100 * y + x;
            }
        }
        return sum;
    }
    
    void run_internal() override
    {
        position start_1;
        long max_x, max_y;
        storage space;
        directions moves;

        input_reader().read_by_line_until_condition_met_or_eof<utility::io::file_reader::read_conditions::empty_line>([&](const auto& line, size_t y)
        {
                for (size_t x = 0; x < line.size(); x++)
                {
                    space[{static_cast<long>(x), static_cast<long>(y)}] = line[x];
                    if (line[x] == '@') start_1 = { static_cast<long>(x) , static_cast<long>(y) };
                }
                max_x = static_cast<long>(line.size());
                max_y = static_cast<long>(y) + 1;
        });

        input_reader().read_by_line_until_condition_met_or_eof<utility::io::file_reader::read_conditions::empty_line>([&](const auto& line, size_t y)
            {
                for (size_t x = 0; x < line.size(); x++)
                {
                    switch (line[x])
                    {
                        case '^': 
                            moves.push_back(direction_2d::up);
                            break;
                        case '>': 
                            moves.push_back(direction_2d::right);
                            break;
                        case '<': 
                            moves.push_back(direction_2d::left);
                            break;
                        case 'v': 
                            moves.push_back(direction_2d::down);
                            break;
                    }
                }
            });

        auto [space_2, max_x_2, max_y_2, start_2] = make_input_twice_wide(space, max_x, max_y);
        
        set_star1_result(calculate_score(solve(space, moves, start_1, 1), max_x, max_y, 'O'));
        set_star2_result(calculate_score(solve(space_2, moves, start_2, 2), max_x_2, max_y_2, '['));
    }
};