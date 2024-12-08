#pragma once
#include "helper.h"

template<>
class day<8, 2022> : public day_base<8,2022>
{
    size_t calculate_score(const std::vector<std::vector<int>>& data, int x_1, int y_1)
    {
        size_t score = 1;
        size_t local = 0;
        for (int x = x_1 + 1; x < data[0].size(); x++)
        {
            local++;
            if (data[y_1][x] >= data[y_1][x_1]) break;
        }
        score *= local;
        local = 0;
        for (int x = x_1 - 1; x >=0; x--)
        {
            local++;
            if (data[y_1][x] >= data[y_1][x_1]) break;
        }
        score *= local;
        local = 0;
        for (int y = y_1 + 1; y < data.size(); y++)
        {
            local++;
            if (data[y][x_1] >= data[y_1][x_1]) break;
        }
        score *= local;
        local = 0;
        for (int y = y_1 - 1; y >= 0; y--)
        {
            local++;
            if (data[y][x_1] >= data[y_1][x_1]) break;
        }
        score *= local;
        return score;
    }
   
    
    
    std::set<std::pair<size_t, size_t>> calculate(const std::vector<std::vector<int>>& data)
        {
            std::set<std::pair<size_t, size_t>> result;
            // up
            for (int i = 0; i < data[0].size(); i++)
            {
                min_max_counter<int> counter;
                for (int j = 0; j < data.size(); j++)
                {
                    if ((data[j][i]) > counter.maximum())
                    {
                        result.insert({ i,j });
                    }
                    counter.check_value(data[j][i]);
                }
            }
            // down
            for (int i = 0; i < data[0].size(); i++)
            {
                min_max_counter<int> counter;
                for (int j = static_cast<int>(data.size())-1; j >=0; j--)
                {
                    if ((data[j][i]) > counter.maximum())
                    {
                        result.insert({ i,j });
                    }
                    counter.check_value(data[j][i]);
                }
            }

            // left
            for (int j = 0; j < data.size(); j++)
            {
                min_max_counter<int> counter;
                for (int i = 0; i< data[0].size(); i++)
                {
                    if ((data[j][i]) > counter.maximum())
                    {
                        result.insert({ i,j });
                    }
                    counter.check_value(data[j][i]);
                }
            }

            // right
            for (int j = 0; j < data.size(); j++)
            {
                min_max_counter<int> counter;
                for (int i = static_cast<int>(data[0].size())-1; i >=0; i--)
                {
                    if ((data[j][i]) > counter.maximum())
                    {
                        result.insert({ i,j });
                    }
                    counter.check_value(data[j][i]);
                }
            }
            return result;
        }
    
    
        void run_internal() override
        {
            std::vector<std::vector<int>> data;
            while (!input_reader().is_end_of_file())
            {
                data.push_back(utility::string::convert_string_of_digits_to_vector<int>(input_reader().get_line()));
            }

            auto set = calculate(data);
            set_star1_result(set.size());

            min_max_counter<size_t> max_view_score;
            for (int y = 0; y < data.size(); y++)
            {
                for (int x = 0; x < data[0].size(); x++)
                {
                    max_view_score.check_value(calculate_score(data, x, y));
                }
            }

            set_star2_result(max_view_score.maximum());
        }
};