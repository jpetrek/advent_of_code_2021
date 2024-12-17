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
        direction::name dir;
        size_t steps;
    };

    const std::map<char, direction::name> mapping = { {'U', direction::up }, {'D', direction::down }, {'R', direction::right }, {'L', direction::left } };

    std::pair<std::vector<record>, std::vector<record>> parse_input()
    {
        std::vector<record> records_s1;
        std::vector<record> records_s2;

        while (!input_reader().is_end_of_file())
        {
            auto l = utility::string::split(input_reader().get_line(), ' ');

            record r1;
            r1.dir = mapping.at(l[0][0]);
            r1.steps = std::stoull(l[1]);
            records_s1.push_back(r1);

            record r2;
            auto n = std::stoul(l[2].substr(7, 1));
            if (n == 0) r2.dir = direction::right;
            else if (n == 1) r2.dir = direction::down;
            else if (n == 2) r2.dir = direction::left;
            else if (n == 3) r2.dir = direction::up;
            r2.steps = std::stoul(l[2].substr(2, 5), nullptr, 16);
            records_s2.push_back(r2);
        }
        return { records_s1, records_s2 };
    }

    std::pair< std::vector<std::pair<int64_t, int64_t>>, size_t> transform_to_polygon(const std::vector<record>& records)
    {
        size_t length = 0;
        std::vector < std::pair<int64_t, int64_t>> result = { { 0,0 } };
        for (const auto r : records)
        {
            length += r.steps;
            if (r.dir == direction::right)
            {
                result.push_back({ result[result.size() - 1].first + r.steps, result[result.size() - 1].second });
            }
            else if (r.dir == direction::left)
            {
                result.push_back({ result[result.size() - 1].first - r.steps, result[result.size() - 1].second });
            }
            else if (r.dir == direction::up)
            {
                result.push_back({ result[result.size() - 1].first, result[result.size() - 1].second - r.steps });
            }
            else if (r.dir == direction::down)
            {
                result.push_back({ result[result.size() - 1].first, result[result.size() - 1].second + r.steps });
            }
        }
        return{ result , length };
    }

    size_t calculate_polygon_space(const std::pair<std::vector<std::pair<int64_t, int64_t>>, size_t>& data)
    {
        auto& [points, trench_length] = data;
        int64_t s = 0;
        for (size_t i = 0; i < points.size(); i++)
        {
            auto i_minus_1 = i == 0 ? points.size() - 1 : i - 1;
            auto i_plus_1 = (i + 1) % points.size();
            s += points[i].first * (points[i_plus_1].second - points[i_minus_1].second);
        }

        return 1 + (abs(s) + trench_length) / 2;
    }

    std::pair< horizontal_lines_t, vertical_lines_t> transform(const std::vector<record>& records)
    {
        horizontal_lines_t horizontal_lines;
        vertical_lines_t vertical_lines;
        int64_t x = 0;
        int64_t y = 0;
        for (const auto r : records)
        {
            if (r.dir == direction::right)
            {
                if (!horizontal_lines.contains(y)) horizontal_lines[y] = {};
                horizontal_lines[y].insert({ x, x + r.steps });
                x += r.steps;
            }
            else if (r.dir == direction::left)
            {
                if (!horizontal_lines.contains(y)) horizontal_lines[y] = {};
                horizontal_lines[y].insert({ x - r.steps, x });
                x -= r.steps;
            }
            else if (r.dir == direction::up)
            {
                vertical_lines[{y - r.steps, x}] = y;
                y -= r.steps;
            }
            else if (r.dir == direction::down)
            {
                vertical_lines[{y, x}] = y + r.steps;
                y += r.steps;
            }
        }
        return{ horizontal_lines , vertical_lines };
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

    bool is_test() const override
    {
        return false;
    }

    void run_internal() override
    {
        auto [lines_s1, lines_s2] = parse_input();

        auto s1_math = calculate_polygon_space(transform_to_polygon(lines_s1));
        auto s2_math = calculate_polygon_space(transform_to_polygon(lines_s2));

        auto s1_rectangles = calculate_space(transform(lines_s1));
        auto s2_rectangles = calculate_space(transform(lines_s2));

        if (s1_math == s1_rectangles) set_star1_result(s1_rectangles);
        if (s2_math == s2_rectangles) set_star2_result(s2_rectangles);
    }
};

