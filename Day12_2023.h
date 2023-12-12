#pragma once
#include "helper.h"

template<>
class day<12, 2023> : public day_base<12,2023>
{
    size_t calc(const std::string input, const std::vector<size_t> nums, std::map<std::pair<std::string, std::vector<size_t>>, size_t>& cache)
    {
        if (input.size() == 0) return nums.size()==0 ? 1 : 0;
        if (nums.size() == 0) return !contains(input, '#') ? 1 :0;

        size_t result = 0;

        if (cache.contains({ input,nums })) return cache.at({ input,nums });

        if ((input[0] == '.') || (input[0] == '?')) result += calc(input.substr(1), nums, cache);

        if ((input[0] == '#') || (input[0] == '?'))
        {
            if (nums[0]<= input.size() && !contains(input.substr(0, nums[0]), '.') && (input.size() == nums[0] || input[nums[0]] != '#'))
            {
                result += calc(helper::slice(input, nums[0] + 1), helper::slice(nums,1), cache);
            }
        }

        cache[{input,nums}] = result;
        return result;
    }

    void run_internal() override
    {
        size_t s1 = 0;
        size_t s2 = 0;
        while (!input_reader().is_end_of_file())
        {
            //set_is_test(true);
            auto line = helper::split(input_reader().get_line(), ' ');
            auto line20 = line[0] + "?" + line[0] + "?" + line[0] + "?" + line[0] + "?" + line[0];
            auto line21 = line[1] + "," + line[1] + "," + line[1] + "," + line[1] + "," + line[1];

            std::map<std::pair<std::string, std::vector<size_t>>, size_t> cache1;
            s1 += calc(line[0], helper::split_convert_vector<size_t>(line[1], ',', helper::stoul()), cache1);
            std::map<std::pair<std::string, std::vector<size_t>>, size_t> cache2;
            s2 += calc(line20, helper::split_convert_vector<size_t>(line21, ',', helper::stoul()), cache2);
        }
        set_star1_result(s1);
        set_star2_result(s2);
    }
};