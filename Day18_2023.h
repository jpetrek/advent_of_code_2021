#pragma once
#include "helper.h"

template<>
class day<18, 2023> : public day_base<18, 2023>
{
    typedef point_2d_generic<int64_t> point_2d;
    typedef std::map<int64_t, std::set<std::pair<int64_t, int64_t>>> horizontal_lines_t;
    typedef std::map<std::pair<int64_t, int64_t>, int64_t> vertical_lines_t;
    struct record
    {
        direction_2d::name dir;
        size_t steps;
    };

    const std::map<char, direction_2d::name> mapping = { {'U', direction_2d::up }, {'D', direction_2d::down }, {'R', direction_2d::right }, {'L', direction_2d::left } };

    bool is_test() const override
    {
        return false;
    }

    std::pair< horizontal_lines_t, vertical_lines_t> transform(const std::vector<record>& records)
    {
        horizontal_lines_t horizontal_lines;
        vertical_lines_t vertical_lines;
        int64_t x = 0;
        int64_t y = 0;
        for (const auto r : records)
        {
            if (r.dir == direction_2d::right)
            {
                if (!horizontal_lines.contains(y)) horizontal_lines[y] = {};
                horizontal_lines[y].insert({ x, x + r.steps });
                x += r.steps;
            }
            else if (r.dir == direction_2d::left)
            {
                if (!horizontal_lines.contains(y)) horizontal_lines[y] = {};
                horizontal_lines[y].insert({ x - r.steps, x });
                x -= r.steps;
            }
            else if (r.dir == direction_2d::up)
            {
                vertical_lines[{y - r.steps, x}] = y;
                y -= r.steps;
            }
            else if (r.dir == direction_2d::down)
            {
                vertical_lines[{y, x}] = y + r.steps;
                y += r.steps;
            }
        }
        return{ horizontal_lines , vertical_lines };
    }

    std::pair<std::pair<horizontal_lines_t, vertical_lines_t>, std::pair<horizontal_lines_t, vertical_lines_t>> parse_input()
    {
        std::vector<record> records_s1;
        std::vector<record> records_s2;

        while (!input_reader().is_end_of_file())
        {
            auto l = helper::split(input_reader().get_line(), ' ');
            
            record r1;
            r1.dir = mapping.at(l[0][0]);
            r1.steps = std::stoull(l[1]);
            records_s1.push_back(r1);

            record r2;
            auto n = std::stoul(l[2].substr(7, 1));
            if (n == 0) r2.dir = direction_2d::right;
            else if (n == 1) r2.dir = direction_2d::down;
            else if (n == 2) r2.dir = direction_2d::left;
            else if (n == 3) r2.dir = direction_2d::up;
            r2.steps = std::stoul(l[2].substr(2, 5), nullptr, 16);
            records_s2.push_back(r2);
        }
        return { transform(records_s1), transform(records_s2) };
    }
    
    size_t calculate_space(const std::pair<horizontal_lines_t, vertical_lines_t>& space)
    {
        auto [horizontal_lines, vertical_lines] = space;
        size_t total_space = 0;

        auto horizontal_lines_view = std::views::keys(horizontal_lines);
        std::vector<int64_t> ys{ horizontal_lines_view.begin(), horizontal_lines_view.end() };

        while (vertical_lines.size() > 0)
        {
            int64_t top_y = ys[0];
            ys.erase(ys.begin());

            std::vector <std::pair<std::pair<int64_t, int64_t>, int64_t >> vertical_lines_interacting_with_top;
            while (vertical_lines.size() > 0 && ((vertical_lines.begin()->first.first <= top_y) && (vertical_lines.begin()->second >= top_y)))
            {
                vertical_lines_interacting_with_top.push_back(*vertical_lines.begin());
                vertical_lines.erase(vertical_lines.begin());
            }

            int64_t bottom_y = ys[0];

            for (size_t i = 0; i < vertical_lines_interacting_with_top.size() - 1; i += 2)
            {
                total_space += (vertical_lines_interacting_with_top[i + 1].first.second - vertical_lines_interacting_with_top[i].first.second + 1) * (bottom_y - top_y);
                
                vertical_lines_interacting_with_top[i].first.first = bottom_y;
                vertical_lines_interacting_with_top[i + 1].first.first = bottom_y;

                for (const auto& l : horizontal_lines.at(bottom_y))
                {
                    bool start_found = vertical_lines_interacting_with_top[i].first.second == l.first;
                    bool end_found = vertical_lines_interacting_with_top[i + 1].first.second == l.second;
                    
                    if (start_found && end_found) total_space += l.second - l.first + 1;
                    else if ((!start_found && end_found) || (start_found && !end_found)) total_space += l.second - l.first;
                    else if (!start_found && !end_found && (vertical_lines_interacting_with_top[i].first.second < l.first) && (vertical_lines_interacting_with_top[i + 1].first.second > l.second))
                    {
                        total_space += l.second - l.first - 1;
                    }
                }
            }
            
            for (size_t i = 0; i < vertical_lines_interacting_with_top.size(); i++)
            {
                if (vertical_lines_interacting_with_top[i].first.first < vertical_lines_interacting_with_top[i].second) vertical_lines[vertical_lines_interacting_with_top[i].first] = vertical_lines_interacting_with_top[i].second;
            }
        }
        return total_space;
    }

    void run_internal() override
    {
        auto [lines_s1, lines_s2] = parse_input();
        set_star1_result(calculate_space(lines_s1));
        set_star2_result(calculate_space(lines_s2));
    }
};

