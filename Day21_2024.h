#pragma once
#include "helper.h"
using namespace utility::arrays;
using namespace utility::string;
using namespace utility::math;

template<>
class day<21, 2024> : public day_base<21, 2024>
{
    typedef std::vector<std::vector<char>> maze;
    typedef point_2d_generic<long> coords;
    typedef direction_2d_generic<long>::name dir;
    const std::vector<direction::name> directions = { direction::up, direction::right, direction::down, direction::left };
    const std::map< direction::name, char> dir_map = { {direction::up, '^'}, {direction::right, '>'}, {direction::down, 'v'}, {direction::left,'<'}};

    const std::vector<std::vector<char>> keybard_numeric =
    {
        {'7','8','9'},
        {'4','5','6'},
        {'1','2','3'},
        {'#','0','A'}
    };

    const std::vector<std::vector<char>> keybard_arrows =
    {
        {'#','^','A'},
        {'<','v','>'}
    };

    coords find(const std::vector<std::vector<char>>& mapping, char c)
    {
        for (long y = 0; y < mapping.size(); y++)
        {
            for (long x = 0; x < mapping[0].size(); x++)
            {
                if (mapping[y][x] == c) return { x,y };
            }
        }
        throw;
    }

    std::string generate_path_1(char start, char finish, const std::vector<std::vector<char>>& mapping)
    {
        if (start == finish) return "";
        auto ps = find(mapping, start);
        auto pe = find(mapping, finish);
        std::map<coords, size_t> v;
        std::vector< direction::name> ret(1000, direction::up);
        traverse(mapping, ps, pe, v, {}, ret);
        std::string s;
        for (const auto& p : ret)
        {
            s.push_back(dir_map.at(p));
        }

        std::sort(std::begin(s), std::end(s));
        return s;
    }

    std::string generate_path(char start, char finish, const std::vector<std::vector<char>>& mapping)
    {
        if (start == finish) return "";
        auto hole = find(mapping, '#');
        auto ps = find(mapping, start);
        auto pe = find(mapping, finish);
        long dw = pe.x - ps.x;
        long dh = pe.y - ps.y;
        std::string ret;
        if (pe.y == hole.y)
        {
            if (dw > 0) for (int i = 0; i < abs(dw); i++) ret.push_back('>');
            else for (int i = 0; i < abs(dw); i++) ret.push_back('<');
            if (dh > 0) for (int i = 0; i < abs(dh); i++) ret.push_back('v');
            else for (int i = 0; i < abs(dh); i++) ret.push_back('^');
        }
        else if (ps.y == hole.y)
        {
            if (dh > 0) for (int i = 0; i < abs(dh); i++) ret.push_back('v');
            else for (int i = 0; i < abs(dh); i++) ret.push_back('^');
            if (dw > 0) for (int i = 0; i < abs(dw); i++) ret.push_back('>');
            else for (int i = 0; i < abs(dw); i++) ret.push_back('<');
        }
        else
        {
            if (dw > 0) for (int i = 0; i < abs(dw); i++) ret.push_back('>');
            else for (int i = 0; i < abs(dw); i++) ret.push_back('<');
            if (dh > 0) for (int i = 0; i < abs(dh); i++) ret.push_back('v');
            else for (int i = 0; i < abs(dh); i++) ret.push_back('^');
        }
        return ret;
    }


    void traverse(const std::vector<std::vector<char>>& mapping, coords pos, coords end, std::map<coords, size_t>& v, std::vector< direction::name> path, std::vector< direction::name>& ret)
    {
        if (pos == end)
        {
            if (path.size() < ret.size()) ret = path;
            return;
        }

        if (v.contains(pos))
        {
            if (v.at(pos) < path.size()) return;
        }
        v[pos] = path.size();

        for (const auto d : directions)
        {
            auto new_pos = add(pos, d);
            path.push_back(d);
            if (is_inside_2D_array<long>(mapping, new_pos))
            {
                traverse(mapping, new_pos, end, v, path, ret);
            }
            path.erase(std::end(path) - 1);
        }
    }

    void test_generate(const std::vector<std::vector<char>>& map)
    {
        for (size_t a = 0; a < map.size() * map[0].size(); a++)
        {
            for (size_t b = 0; b < map.size() * map[0].size(); b++)
            {
                char s = map[a / map[0].size()][a % map[0].size()];
                char f = map[b / map[0].size()][b % map[0].size()];
                auto p = generate_path(s, f, map);
                std::cout << s << " >> " << f << " : " << p << std::endl;
            }
        }
    }

    inline bool is_test() const override { return false; }

    void run_internal() override
    {
        size_t sum_1 = 0;
        input_reader().read_by_line_until_condition_met_or_eof<long>(file_reader::empty_line_condition(), [&](const auto& line, const long y)
        {
            long number;
            std::string n = line;
            n.erase(n.size() - 1);
            number = std::stol(n);
            std::string output = line;
            size_t i = 0;
            while (i < 3)
            {
                char pos = 'A';
                std::string new_out;
                if (i == 0) for (const auto c : output)
                {
                    auto s = generate_path(pos, c, keybard_numeric);
                    new_out += s + 'A';
                    pos = c;
                }
                else if (i < 3) for (const auto c : output)
                {
                    auto s = generate_path(pos, c, keybard_arrows);
                    new_out += s + 'A';
                    pos = c;
                }
                output = new_out;
                i++;
            }
            sum_1 += number * output.size();
        });

        set_star1_result(sum_1);
//       set_star2_result(get_nr_of_shortcuts_over(path_points, 20));
    }
};