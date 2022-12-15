#pragma once
#include "helper.h"

template<>
class day<5, 2021> : public day_base<5, 2021>
{
    void run_internal() override
    {
        struct line
        {
            int x1;
            int y1;
            int x2;
            int y2;
        };

        std::vector<line> lines;
        int maxX = 0;
        int maxY = 0;
        int minX = 100000;
        int minY = 100000;
        while (!input_reader().is_end_of_file())
        {
            int x1, x2, y1, y2;
            sscanf_s(input_reader().get_line().c_str(), "%d,%d -> %d,%d", &x1, &y1, &x2, &y2);
            if (x1 > maxX) maxX = x1;
            if (x2 > maxX) maxX = x2;
            if (y1 > maxY) maxY = y1;
            if (y2 > maxY) maxY = y2;

            if (x1 < minX) minX = x1;
            if (x2 < minX) minX = x2;
            if (y1 < minY) minY = y1;
            if (y2 < minY) maxY = y2;

            lines.push_back({ x1, y1, x2, y2 });
        }

        std::vector<std::vector<size_t>> field(maxY + 1, std::vector<size_t>(maxX + 1, 0));
        for (const auto& l : lines)
        {
            if (l.x1 == l.x2)
            {
                int dir = l.y1 < l.y2 ? 1 : -1;
                for (int y = l.y1; y != l.y2; y += dir)
                {
                    field[y][l.x1]++;
                }

                field[l.y2][l.x1]++;
            }

            else if (l.y1 == l.y2)
            {
                int dir = l.x1 < l.x2 ? 1 : -1;
                for (int x = l.x1; x != l.x2; x += dir)
                {
                    field[l.y1][x]++;
                }
                field[l.y1][l.x2]++;
            }
        }

        size_t sum = 0;
        for (const auto& r : field)
            for (const auto& i : r)
            {
                if (i > 1) sum++;
            }
        set_star1_result(sum);

        for (const auto& l : lines)
        {
            if ((l.x1 != l.x2) && (l.y1 != l.y2))
            {
                int dirx = l.x1 < l.x2 ? 1 : -1;
                int diry = l.y1 < l.y2 ? 1 : -1;

                int y = l.y1;
                for (int x = l.x1; x != l.x2; x += dirx)
                {
                    field[y][x]++;
                    y += diry;
                }
                field[l.y2][l.x2]++;
            }
        }

        sum = 0;
        for (const auto& r : field)
            for (const auto& i : r)
            {
                if (i > 1) sum++;
            }
        set_star2_result(sum);
    }
};
