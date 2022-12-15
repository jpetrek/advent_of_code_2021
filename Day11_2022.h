#pragma once
#include "helper.h"

template<>
class day<11, 2022> : public day_base<11,2022>
{
    enum class operation
    {
        plus = 0,
        mult = 1
    };

    enum class operand
    {
        old = 0,
        number = 1
    };

    struct monkey
    {
        size_t index;
        std::vector<size_t> items;
        operation o;
        operand op1, op2;
        size_t op1_value, op2_value;
        size_t divisible_by;
        size_t where_to_throw_if_true;
        size_t where_to_throw_if_false;
    };

    void process_monkey(size_t current, std::vector<monkey>& monkeys, std::vector<size_t>& inspects, size_t normalize)
    {
        auto& m = monkeys[current];
        inspects[current] += m.items.size();
        for (auto i : m.items)
        {
            size_t operand2 = m.op2 == operand::old ? i : m.op2_value;
            size_t new_wl = m.o == operation::plus ? i + operand2 : i * operand2;
            new_wl = normalize == 0 ? new_wl / 3 : new_wl % normalize;
            
            if (new_wl % m.divisible_by == 0)
            {
                monkeys[m.where_to_throw_if_true].items.push_back(new_wl);
            }
            else
            {
                monkeys[m.where_to_throw_if_false].items.push_back(new_wl);
            }
        }
        m.items.clear();
    }

    std::pair<std::vector<monkey>, size_t> create_monkey_set_and_common_divisibles_multiple()
    {
        std::vector<monkey> monkeys;
        std::set<size_t> divisors;
        while (!input_reader().is_end_of_file())
        {
            monkey m;
            m.index = std::stoull(helper::split(input_reader().get_line(), ' ')[1]);
            m.items = helper::split_and_convert<size_t>(helper::split(input_reader().get_line(), ':')[1], ',');
            auto oper = helper::split(input_reader().get_line(), ' ');
            m.op1 = oper[3] == "old" ? operand::old : operand::number;
            m.o = oper[4] == "+" ? operation::plus : operation::mult;
            m.op2 = oper[5] == "old" ? operand::old : operand::number;
            if (m.op1 == operand::number) m.op1_value = std::stol(oper[3]);
            if (m.op2 == operand::number) m.op2_value = std::stol(oper[5]);
            m.divisible_by = std::stol(helper::split(input_reader().get_line(), ' ')[3]);
            divisors.insert(m.divisible_by);
            m.where_to_throw_if_true = std::stol(helper::split(input_reader().get_line(), ' ')[5]);
            m.where_to_throw_if_false = std::stol(helper::split(input_reader().get_line(), ' ')[5]);
            monkeys.push_back(m);
            input_reader().get_line();
        }

        return { monkeys, std::accumulate(std::begin(divisors), std::end(divisors),static_cast<size_t>(1), std::multiplies<size_t>()) };
    }

    void run_internal() override
    {
        auto [monkeys, norm] = create_monkey_set_and_common_divisibles_multiple();
        auto  monkeys2 = monkeys;
        std::vector<size_t> inspects1(monkeys.size(), 0);
        for (size_t i = 0; i < 20; i++)
        {
            for (size_t m = 0; m < monkeys.size(); m++)
            {
                process_monkey(m, monkeys, inspects1, 0);
            }
        }
        std::sort(inspects1.begin(), inspects1.end());
        set_star1_result(inspects1.back() * inspects1[inspects1.size() - 2]);

        std::vector<size_t> inspects2(monkeys2.size(), 0);
        for (size_t i = 0; i < 10000; i++)
        {
            for (size_t m = 0; m < monkeys2.size(); m++)
            {
                process_monkey(m, monkeys2, inspects2, norm);
            }
        }
        std::sort(inspects2.begin(), inspects2.end());
        set_star2_result(inspects2.back() * inspects2[inspects2.size() - 2]);
    }
};