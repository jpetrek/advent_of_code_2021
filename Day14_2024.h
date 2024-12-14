#pragma once
#include "helper.h"

template<>
class day<14, 2024> : public day_base<14,2024>
{
    typedef point_2d_generic<long> coords;
    typedef direction_2d_generic<long>::diference increments;

    struct robot
    {
        coords start;
        increments move;
    };

    coords calculate_position_after_moves(const robot& r, const long max_x, const long max_y, size_t number_of_moves)
    {
        long nx, ny;
        if (r.move.dx >= 0) nx = (r.start.x + r.move.dx * number_of_moves) % max_x;
        else nx = max_x - (((max_x - 1 - r.start.x) - (r.move.dx * number_of_moves)) % max_x) - 1;
        if (r.move.dy >= 0) ny = (r.start.y + r.move.dy * number_of_moves) % max_y;
        else ny = max_y - (((max_y - 1 - r.start.y) - (r.move.dy * number_of_moves)) % max_y) - 1;
        return { nx, ny };
    }

    size_t quadrant_id(coords pos, size_t max_x, size_t max_y)
    {
        size_t bx = max_x / 2;
        size_t by = max_y / 2;
        if ((pos.x < bx) && (pos.y < by)) return 0;
        if ((pos.x > bx) && (pos.y < by)) return 1;
        if ((pos.x < bx) && (pos.y > by)) return 2;
        if ((pos.x > bx) && (pos.y > by)) return 3;
        return 4;
    }

    std::vector<double> calculate_average_distances_from_centre(const std::vector<robot>& robots, size_t max_x, size_t max_y, size_t max_number_of_steps)
    {
        std::vector<double> average_distances_from_centre;
        for (size_t i = 0; i < max_number_of_steps; i++)
        {
            double psd_x = 0;
            double psd_y = 0;
            for (const auto& r : robots)
            {
                auto nx = calculate_position_after_moves(r, static_cast<long>(max_x), static_cast<long>(max_y), i);
                psd_x += abs(static_cast<long>(max_x) / 2 - nx.x);
                psd_y += abs(static_cast<long>(max_y) / 2 - nx.y);
            }
            average_distances_from_centre.push_back(psd_x / robots.size() * psd_y / robots.size());
        }
        return average_distances_from_centre;
    }

    void print_just_for_fun(const std::vector<robot>& robots, size_t max_x, size_t max_y, size_t max_number_of_steps)
    {
        std::map<coords, bool> space;
        for (const auto& r : robots)
        {
            auto nx = calculate_position_after_moves(r, static_cast<long>(max_x), static_cast<long>(max_y), max_number_of_steps);
            space[nx] = true;
        }

        for (long y = 0; y < max_y; y++)
        {
            for (long x = 0; x < max_x; x++)
            {
                if (space[{x, y}]) std::cout << '*';
                else std::cout << ' ';
            }
            std::cout << std::endl;
        }
    }

    inline bool is_test() const override { return false; }
    void run_internal() override
    {
        std::vector<robot> robots;
        long max_x = is_test() ? 11 : 101;
        long max_y = is_test() ? 7 : 103;

        input_reader().read_by_line_until_condition_met_or_eof<utility::io::file_reader::read_conditions::empty_line>([&](const auto& line)
        {
            robot r;
            sscanf_s(line.c_str(), "p=%ld,%ld v=%ld,%ld", &r.start.x, &r.start.y, &r.move.dx, &r.move.dy);
            robots.push_back(r);
        });

        std::vector<size_t> quadrants_and_centre = { 0,0,0,0,0 };

        for (const auto& r : robots)
        {
            auto nx = calculate_position_after_moves(r, max_x, max_y, 100);
            quadrants_and_centre[quadrant_id(nx, max_x, max_y)]++;
        }

        auto ad = calculate_average_distances_from_centre(robots, max_x, max_y, 20000);
        //print_just_for_fun(robots, max_x, max_y, std::distance(std::begin(ad), std::min_element(std::begin(ad), std::end(ad))));

        set_star1_result(quadrants_and_centre[0]* quadrants_and_centre[1] * quadrants_and_centre[2] * quadrants_and_centre[3]);
        set_star2_result(std::distance(std::begin(ad), std::min_element(std::begin(ad), std::end(ad))));
    }
};