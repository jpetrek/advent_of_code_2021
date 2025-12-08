#pragma once
#include "helper.h"
using namespace utility::io;

template<>
class day<16, 2024> : public day_base<16,2024>
{
    typedef point_2d_generic<long> position;
    typedef direction_2d_generic<long> direction;

    typedef std::map<position, char> area_map;
    typedef std::map<position, long> score_board_simple;
    typedef std::map<uint64_t, long> score_board;
    typedef std::set<position> unique_points;

    const std::map<direction::name, std::vector<std::pair<direction::name, long>>> moves =
    {
        {direction::up, {{direction::up, 1},{direction::right, 1001},{direction::left, 1001}}},
        {direction::down, {{direction::down, 1},{direction::right, 1001},{direction::left, 1001}}},
        {direction::left, {{direction::left, 1},{direction::up, 1001},{direction::down, 1001}}},
        {direction::right, {{direction::right, 1},{direction::up, 1001},{direction::down, 1001}}}
    };

    inline uint64_t hash(position p, direction::name d) const { return (static_cast<uint64_t>(p.x) << 32) + (static_cast<uint64_t>(p.y) << 16) + d;}

    void traverse_fast(const area_map& maze, const position actual, const direction::name actual_direction, const position finish, const long actual_score, score_board_simple& score) const
    {
        if (score.contains(actual) && (score.at(actual) <= actual_score)) return;
        score[actual] = actual_score;
        if (actual == finish) return;
        for (const auto& m : moves.at(actual_direction))
        {
            auto new_postion = add(actual, m.first);
            if (maze.at(new_postion) != '#')
            {
                traverse_fast(maze, new_postion, m.first, finish, actual_score + m.second, score);
            }
        }
    }

    uint64_t calculate_star_1(const area_map& maze, const position start, const position finish) const
    {
        score_board_simple board;
        traverse_fast(maze, start, direction::right, finish, 0, board);
        return board.at(finish);
    }

    bool traverse(const uint64_t best_path_score, const area_map& maze, const position actual, const direction::name actual_direction, const position finish, const long actual_score, score_board& score, unique_points& points) const
    {
        bool ret = false;
        auto h = hash(actual, actual_direction);
        if (score.contains(h) && (score.at(h) < actual_score)) return ret;
        score[h] = actual_score;
        
        if (actual == finish) return actual_score == best_path_score;

        for (const auto& m : moves.at(actual_direction))
        {
            auto new_postion = add(actual, m.first);
            if (maze.at(new_postion) != '#')
                {
                if (traverse(best_path_score, maze, new_postion, m.first, finish, actual_score + m.second, score, points))
                {
                    ret = true;
                    points.insert(new_postion);
                }
            }
        }

        return ret;
    }

    uint64_t calculate_star_2(const area_map& maze, const position start, const position finish, const uint64_t best_path_score) const
    {
        score_board board;
        unique_points points = {start};
        traverse(best_path_score, maze, start, direction::right, finish, 0, board, points);
        return points.size();
    }

    inline bool is_test() const override { return false; }

    void run_internal() override
    {
        area_map maze;
        position start, finish;
        
        input_reader().read_by_line_until_condition_met_or_eof<long>(file_reader::empty_line_condition(), [&](const auto& line, const long y)
        {
            for (long x = 0; x < line.size(); x++)
            {
                maze[{x, y}] = line[x];
                if (line[x] == 'S') start = { x,y };
                if (line[x] == 'E') finish = { x,y };
            }
        });

        auto val_1 = calculate_star_1(maze, start, finish);
        auto val_2 = calculate_star_2(maze, start, finish, val_1);
        
        set_star1_result(val_1);
        set_star2_result(val_2);
    }
};