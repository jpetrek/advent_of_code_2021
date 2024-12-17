#pragma once
#include "helper.h"
using namespace utility::io;

template<>
class day<17, 2024> : public day_base<17, 2024>
{
    uint64_t get_combo_op(const uint8_t op, const uint64_t A, const uint64_t B, const uint64_t C) const
    {
        if ((op >= 0) && (op <= 3))  return op;
        if (op == 4) return A;
        if (op == 5) return B;
        if (op == 6) return C;
        throw;
    }

    std::string process(uint64_t register_A, uint64_t register_B, uint64_t register_C, const std::vector<uint8_t>& program) const
    {
        std::string output;
        size_t i_p = 0;
        while (i_p < program.size())
        {
            if (program[i_p] == 0) //adv
            {
                uint64_t denominator = static_cast<uint64_t>(pow(2, get_combo_op(program[i_p + 1], register_A, register_B, register_C)));
                register_A = register_A / denominator;
                i_p += 2;
            }
            else if (program[i_p] == 1) //bxl
            {
                register_B = register_B ^ static_cast<uint64_t>(program[i_p + 1]);
                i_p += 2;
            }
            else if (program[i_p] == 2) //bst
            {
                register_B = get_combo_op(program[i_p + 1], register_A, register_B, register_C) % 8;
                i_p += 2;
            }
            else if (program[i_p] == 3) //jnz
            {
                if (register_A == 0)
                {
                    i_p += 2;
                }
                else
                {
                    i_p = program[i_p + 1];
                }
            }
            else if (program[i_p] == 4) //bxc
            {
                register_B = register_B ^ register_C;
                i_p += 2;
            }
            else if (program[i_p] == 5) //out
            {
                output += std::to_string(get_combo_op(program[i_p + 1], register_A, register_B, register_C) % 8);
                i_p += 2;
            }
            else if (program[i_p] == 6) //bdv
            {
                uint64_t denominator = static_cast<uint64_t>(pow(2, get_combo_op(program[i_p + 1], register_A, register_B, register_C)));
                register_B = register_A / denominator;
                i_p += 2;
            }
            else if (program[i_p] == 7) //cdv
            {
                uint64_t denominator = static_cast<uint64_t>(pow(2, get_combo_op(program[i_p + 1], register_A, register_B, register_C)));
                register_C = register_A / denominator;
                i_p += 2;
            }
        }
        return output;
    }

    std::vector < std::vector<uint64_t>> generate_patterns(const uint8_t number) const
    {
        // this one is tailored to my input, not generic!
        uint64_t b0 = number & 1;
        uint64_t b1 = (number >> 1) & 1;
        uint64_t b2 = (number >> 2) & 1;
        
        std::vector < std::vector<uint64_t>> ret;
        if (number == 6)  ret.push_back({ 2,2,2,2,2,2,2,0,1,1 });
        if ((b0 == 1) && (b1 == 0)) ret.push_back({ 2,2,2,2,2,2,1 ^ b2,0,1,0 });
        if (b0 == 1) ret.push_back({ 2,2,2,2,2,1^b2,1 ^ b1,0,0,1 });
        //                      a9      a8        a7       a6           a5         a4       a3         a2      a1      a0   
        ret.push_back({          2,      2,        2,       2,        1^b2,      1^b1,    0^b0,         0,      0,      0});
        ret.push_back({          2,      2,        2,    0^b2,        0^b1,      1^b0,       2,         1,      1,      1 });
        ret.push_back({          2,      2,     0^b2,    0^b1,        0^b0,         2,       2,         1,      1,      0 });
        ret.push_back({          2,   0^b2,     1^b1,    1^b0,           2,         2,       2,         1,      0,      1 });
        ret.push_back({       0^b2,   1^b1,     0^b0,       2,           2,         2,       2,         1,      0,      0 });
        return ret;
    }

    std::vector<std::vector<uint64_t>> combine(const std::vector< uint8_t>& numbers) const
    {
        // Tailored to my input, in my case every digit requires +7bits to calculate for all possible combinations and shift is 3
        size_t shift = 3;
        size_t s = numbers.size() * shift + 7; 
        
        size_t i = 0;
        std::vector<std::vector<uint64_t>> possibilities = { std::vector<uint64_t>(s, 2) };
        
        for (auto n : numbers)
        {
            auto current = generate_patterns(n);
            std::vector<std::vector<uint64_t>> new_possibilities;
            for (auto possibility : possibilities)
            {
                for (auto one : current)
                {
                    auto copy = possibility;
                    bool check = true;
                    for (size_t j = 0; j < one.size(); j++)
                    {
                        size_t index_store = i+j;
                        size_t index_cur = one.size() - 1 - j;

                        if (((possibility[index_store] == 1) && (one[index_cur] == 0)) || ((possibility[index_store] == 0) && (one[index_cur] == 1)))
                        {
                            check = false;
                            break;
                        }
                        else
                        {
                            if (one[index_cur] != 2 ) copy[index_store] = one[index_cur];
                        }
                    }
                    if (check) new_possibilities.push_back(copy);
                }
            }
            possibilities = new_possibilities;
            i += shift;
        }
        return possibilities;
    }

    inline bool is_test() const override { return false; }

    void run_internal() override
    {
        uint64_t register_A;
        uint64_t register_B;
        uint64_t register_C;
        size_t i_p = 0;

        auto A = split(input_reader().get_line(), ':');
        register_A = std::stoull(A[1]);
        auto B = split(input_reader().get_line(), ':');
        register_B = std::stoull(B[1]);
        auto C = split(input_reader().get_line(), ':');
        register_C = std::stoull(C[1]);
        input_reader().get_line();
        auto P = split(input_reader().get_line(), ':');
        auto program = split_and_convert<uint8_t>(P[1],',', [](const auto& s) {return static_cast<uint8_t>(std::stoul(s)); });
        std::string input; for (const auto a : program) input += std::to_string(a);

        auto s1 = std::stoull(process(register_A, register_B, register_C, program)); // number form

        min_max_counter<uint64_t> mm;
        auto possibilities = combine(program);
        for (auto one : possibilities)
        {
            uint64_t number = 0;
            uint64_t p2 = 1;
            for (long i = 0; i < one.size(); i++)
            {
                number = number + p2 * (one[i] == 1 ? 1 : 0);
                p2 *= 2;
            }
            if (std::stoull(process(number, 0, 0, program)) == std::stoull(input)) mm.check_value(number);
        }

        set_star1_result(s1);
        set_star2_result(mm.minimum());
    }
};