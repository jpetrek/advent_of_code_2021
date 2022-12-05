#pragma once
#include "helper.h"

template<>
class day<11, 2021> : public day_base<11, 2021>
{
    void run_interal() override
    {
        std::vector<std::vector<size_t>> field;

        while (!input_reader().is_end_of_file())
        {
            std::vector<size_t> row;
            auto line = input_reader().get_line();
            for (auto c : line)
            {
                row.push_back(static_cast<size_t>(c) - static_cast<size_t>('0'));
            }
            field.push_back(row);
        }

        std::size_t total_number_of_flashes = 0;
        for (size_t step = 0; step < 1000; step++)
        {
            for (auto& r : field)
                for (auto& i : r)
                {
                    i++;
                }

            bool was_flash = true;
            std::set<size_t> already_flashed;
            while (was_flash)
            {
                was_flash = false;
                for (size_t y = 0; y < field.size(); y++)
                {
                    for (size_t x = 0; x < field[y].size(); x++)
                    {
                        size_t index = y * field[y].size() + x;
                        if ((already_flashed.find(index) == std::end(already_flashed)) && (field[y][x] > 9))
                        {
                            was_flash = true;
                            already_flashed.insert(index);
                            helper::do_for_adjacent(field[y].size(), field.size(), x, y, [&](size_t lx, size_t ly)
                            {
                                field[ly][lx]++;
                            });
                        }
                    }
                }
            }
            total_number_of_flashes += already_flashed.size();

            for (auto& r : field)
                for (auto& i : r)
                {
                    if (i > 9) i = 0;
                }

            bool sync = true;
            for (auto& r : field)
            {
                for (auto& i : r)
                {
                    if (i != 0)
                    {
                        sync = false;
                        break;
                    }
                }
                if (sync == false) break;
            }

            if (sync)
            {
                set_star2_result(step + 1);
                break;
            }

            if (step == 99) set_star1_result(total_number_of_flashes);
        }
    }
};
