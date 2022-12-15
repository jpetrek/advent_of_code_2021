#pragma once
#include "helper.h"

template<>
class day<13, 2021> : public day_base<13, 2021>
{
    void print(const std::set<std::pair<size_t, size_t>>& data)
    {
        auto max_x = std::max_element(std::begin(data), std::end(data), [](const auto& i, const auto& j) { return i.first < j.first;})->first;
        auto max_y = std::max_element(std::begin(data), std::end(data), [](const auto& i, const auto& j) { return i.second < j.second; })->second;
        for (size_t i = 0; i <= max_y; i++)
        {
            for (size_t j = 0; j <= max_x; j++)
            {
                std::cout << (data.find({ j, i }) != data.end() ? '#' : ' ');
            }
            std::cout << std::endl;
        }
    }
    
    
    void run_internal() override
    {
        std::vector<std::pair<size_t, size_t>> folds;
        std::vector<std::pair<size_t, size_t>> input;

        while (!input_reader().is_end_of_file())
        {
            auto line = input_reader().get_line();
            if (line.find("fold along x") != std::string::npos)
            {
                long fn;
                sscanf_s(line.c_str(), "fold along x = %d", &fn);
                folds.push_back({ fn, 0 });
            }
            else if (line.find("fold along y") != std::string::npos)
            {
                long fn;
                sscanf_s(line.c_str(), "fold along y = %d", &fn);
                folds.push_back({ 0, fn });
            }
            else
            {
                long x,y;
                sscanf_s(line.c_str(), "%d,%d", &x, &y);
                input.push_back({x,y});
            }
        }

        std::set<std::pair<size_t, size_t>> points;
        for (const auto& i : input)
        {
            auto x = i.first;
            auto y = i.second;
            for (size_t fi = 0; fi < folds.size(); fi++)
            {
                if (folds[fi].first != 0)
                {
                    x = (folds[fi].first > x) ? x : 2 * folds[fi].first - x;
                }
                else
                {
                    y = (folds[fi].second > y) ? y : 2 * folds[fi].second - y;
                }
            }
            points.insert({x, y});
        }
        print(points);
    }
};
