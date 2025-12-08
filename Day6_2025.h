#pragma once
#include "helper.h"
using namespace utility::io;

template<>
class day<6, 2025> : public day_base<6,2025>
{
    struct op_with_position
    {
        char op;
        size_t min;
        size_t max;
    };

    inline bool is_test() const override { return false; }

    void run_internal() override
    {
        size_t sum_1 = 0;
        size_t sum_2 = 0;
        std::vector<std::vector<size_t>> numbers;
        std::vector < std::string> digits;
        input_reader().read_by_line_until_condition_met_or_eof<size_t>(file_reader::empty_line_condition(), [&](const auto& line, const auto)
            {
                if ((line[0] == '+') || (line[0] == '*'))
                {
                    auto ops = split(line, ' ');
                    for (size_t i = 0; i < ops.size(); i++)
                    {
                        if (ops[i] == "+")
                        {
                            size_t res = 0;
                            for (size_t j = 0; j < numbers.size(); j++)
                            {
                                res += numbers[j][i];
                            }
                            sum_1 += res;
                        }
                        else if (ops[i] == "*")
                        {
                            size_t res = 1;
                            for (size_t j = 0; j < numbers.size(); j++)
                            {
                                res *= numbers[j][i];
                            }
                            sum_1 += res;
                        }
                    }
                    //--------------------------------------------------------------
                    std::vector<op_with_position> calc_spaces;
                    for (size_t pos = 0; pos < line.size(); pos++)
                    {
                        if ((line[pos] == '+') || (line[pos] == '*'))
                        {
                            if (pos > 0) calc_spaces[calc_spaces.size() - 1].max = pos-2;
                            calc_spaces.push_back({ line[pos], pos, pos });
                        }
                    }
                    calc_spaces[calc_spaces.size() - 1].max = line.size() - 1;
                    for (const auto& cs : calc_spaces)
                    {
                        size_t res = cs.op == '+' ? 0 : 1;
                        for (size_t pos = cs.min; pos <= cs.max; pos++)
                        {
                            size_t value = 0;
                            for (size_t y = 0; y < digits.size(); y++)
                            {
                                if (digits[y][pos] != ' ')
                                {
                                    value = value * 10 + digit_char_to_value<size_t>(digits[y][pos]);
                                }
                            }
                            if (cs.op == '+') res += value;
                            else res *= value;
                        }
                        sum_2 += res;
                    }
                }
                else
                {
                    digits.push_back(line);
                    numbers.push_back(split_and_convert<size_t>(line, ' ', utility::string::stoull()));
                }
            });
 
        set_star1_result(sum_1);

        set_star2_result(sum_2);
    }
};