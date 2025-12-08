#pragma once
#include "helper.h"



template<>
class day<1, 2025> : public day_base<1,2025>
{
    inline bool is_test() const override { return false; }
    void run_internal() override
    {
        const long full_rotation = 100;

        size_t s1 = 0;
        size_t s2 = 0;
        long dial = 50;
        char direction;
        long value;

        while (!input_reader().is_end_of_file())
        {
            long old_dial = dial;
            std::stringstream(input_reader().get_line()) >> direction >> value;

            s2 += value / full_rotation;
            dial += (value % full_rotation) * (direction == 'L' ? -1 : 1);
            
            if (((old_dial != 0) && (dial <= 0)) || (dial >= full_rotation)) s2++;

            if (dial < 0) dial += full_rotation;
            if (dial >= full_rotation) dial -= full_rotation;

            if (dial == 0) s1++;
        }

        set_star1_result(s1);
        set_star2_result(s2);
    }
};