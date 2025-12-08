#pragma once
#include "helper.h"
using namespace utility::io;

template<>
class day<15, 2024> : public day_base<15,2024>
{
    typedef point_2d_generic<long> position;
    typedef std::set<std::pair<position, char>> unique_position_and_value;
    typedef direction_2d_generic<long> direction;
    typedef std::vector<direction::name> directions;
    typedef std::map<position, char> storage;

    inline bool is_test() const override { return false; }
    
    std::tuple<storage, long, long, position> make_input_twice_wide(const storage& space, const long max_x, const long max_y) const
    {
        storage space_2;
        position start_2;
        for_2D_rectangle_space<long>(0, max_x, 0, max_y, [&space, &space_2, &start_2](const auto x, const auto y)
        {
            if ((space.at({ x, y }) == '.') || (space.at({ x, y }) == '#'))
            {
                space_2[{x * 2, y}] = space.at({ x, y });
                space_2[{x * 2 + 1, y}] = space.at({ x, y });
            }
            else if (space.at({ x, y }) == '@')
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
        });
        return {space_2, max_x * 2, max_y, start_2};
    }

    void traverse_crates_1(const storage& space, const position& start, const direction::name move, unique_position_and_value& movable) const
    {
        auto new_position = start;
        while ((space.at(new_position) != '#') && (space.at(new_position) != '.'))
        {
            movable.insert({ new_position, space.at(new_position) });
            new_position = add(new_position, move);
        }
    }

    void traverse_crates_2(const storage& space, const position& start, const direction::name move, unique_position_and_value& movable) const
    {
        movable.insert({ start, space.at(start) });
        auto new_position = add(start, move);
        if (space.at(new_position) == '[' || space.at(new_position) == ']')
        {
            traverse_crates_2(space, new_position, move, movable);

            if ((move == direction::up) || (move == direction::down))
            {
                new_position.x += space.at(new_position) == '[' ? 1 : -1;
                traverse_crates_2(space, new_position, move, movable);
            }
        }
    }

    storage solve(const storage& input, const directions& moves, const position& start, const size_t star) const
    {
        auto space = input;
        auto actual = start;
        for (auto move : moves)
        {
            unique_position_and_value movable;
            if (star == 1) traverse_crates_1(space, actual, move, movable);
            else traverse_crates_2(space, actual, move, movable);

            if (std::all_of(std::begin(movable), std::end(movable), [&](const auto& pm) { return (space.at(add(pm.first, move)) != '#'); }))
            {
                for (const auto& m : movable) space[m.first] = '.';
                for (const auto& m : movable) space[add(m.first, move)] = m.second;
                actual = add(actual, move);
            }
        }
        return space;
    }

    uint64_t calculate_score(const storage& space, const long max_x, const long max_y, const char id) const
    {
        uint64_t sum = 0;
        for_2D_rectangle_space<long>(0, max_x, 0, max_y, [&space, &sum, id](const auto x, const auto y)
        {
            if (space.at({ x, y }) == id) sum += 100ull * y + x;
        });
        return sum;
    }
    
    void run_internal() override
    {
        position start_1_pos;
        long max_x, max_y;
        storage space;
        directions moves;

        input_reader().read_by_line_until_condition_met_or_eof<long>(file_reader::empty_line_condition(), [&](const auto& line, const auto y)
        {
            for (long x = 0; x < static_cast<long>(line.size()); x++)
            {
                space[{x,y}] = line[x];
                if (line[x] == '@') start_1_pos = { x ,y };
            }
            max_x = static_cast<long>(line.size());
            max_y = y + 1;
        });

        input_reader().read_by_line_until_condition_met_or_eof<size_t>(file_reader::empty_line_condition(), [&](const auto& line, const auto)
        {
            for (size_t x = 0; x < line.size(); x++)
            {
                moves.push_back(direction::char_to_name(line[x]));
            }
        });

        auto [space_2, max_x_2, max_y_2, start_2_pos] = make_input_twice_wide(space, max_x, max_y);
        
        set_star1_result(calculate_score(solve(space, moves, start_1_pos, 1), max_x, max_y, 'O'));
        set_star2_result(calculate_score(solve(space_2, moves, start_2_pos, 2), max_x_2, max_y_2, '['));
    }
};