#pragma once
#include "helper.h"

template<>
class day<4, 2024> : public day_base<4,2024>
{
    typedef std::vector<std::string> string_array;
    typedef direction_2d_generic<int> d_2D;
    
    bool find_word(const  string_array& data, const std::string& input, int x, int y, d_2D::diference d) const
    {
        bool search_in_progress = true;
        size_t input_index = 0;
        while (search_in_progress)
        {
            if ((x < 0) || (y < 0) || x >= data[0].size() || y >= data.size()) return false;
            if (data[y][x] == input[input_index])
            {
                x += d.dx;
                y += d.dy;
                input_index++;
                if (input_index == input.size()) return true;
            }
            else
            {
                search_in_progress = false;
            }
        }
        return false;
    }
    
    bool check_shape(const string_array& data, const int x, const int y, const std::vector<std::string> shape)
    {
        return find_word(data, shape[0], x, y, d_2D::name_to_diff(d_2D::nw)) &&
               find_word(data, shape[1], x, y, d_2D::name_to_diff(d_2D::ne)) &&
               find_word(data, shape[2], x, y, d_2D::name_to_diff(d_2D::sw)) &&
               find_word(data, shape[3], x, y, d_2D::name_to_diff(d_2D::se));
    }

    inline bool is_test() const override { return false; }
    void run_internal() override
    {
        string_array data;
        
        while (!input_reader().is_end_of_file())
        {
            data.push_back(input_reader().get_line());
        }

        size_t sum_1 = 0;
        for (int i = 0; i < data.size(); i++)
        {
            for (int j = 0; j < data[0].size(); j++)
            {
                d_2D::do_for_directions(d_2D::eight_directions, [&](const auto dif)
                {
                    sum_1 += find_word(data, "XMAS", j, i, dif);
                });
            }
        }

        size_t sum_2 = 0;
        for (int i = 0; i < data.size(); i++)
        {
            for (int j = 0; j < data[0].size(); j++)
            {
                if (check_shape(data, j, i, { "AM","AM","AS","AS" })) sum_2++;
                if (check_shape(data, j, i, { "AS","AM","AS","AM" })) sum_2++;
                if (check_shape(data, j, i, { "AS","AS","AM","AM" })) sum_2++;
                if (check_shape(data, j, i, { "AM","AS","AM","AS" })) sum_2++;
            }
        }

        set_star1_result(sum_1);
        set_star2_result(sum_2);
    }
};