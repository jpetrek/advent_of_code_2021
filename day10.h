#pragma once
#include "helper.h"

class day10 : public day_base<10>
{
    struct chunk
    {
        char opening;
        char closing;
        size_t error;
        size_t score;
    };

    const std::vector<chunk> chunks =
    {
        {'(',')',3,1},
        {'[',']',57,2},
        {'{','}',1197,3},
        {'<','>',25137,4}
    };

    void run_interal() override
    {
        size_t total_error = 0;
        std::vector<size_t> scores;

        while (!input_reader().is_end_of_file())
        {
            std::stack<char> openings;
            size_t line_error = 0;
            for (auto c : input_reader().get_line())
            {
                auto cbi = std::find_if(chunks.cbegin(), chunks.cend(), [&](const auto& b) { return b.closing == c; });
                if (cbi != chunks.cend())
                {
                    if (openings.top() == cbi->opening)
                    {
                        openings.pop();
                    }
                    else
                    {
                        line_error += cbi->error;
                        break;
                    }
                }
                else
                {
                    openings.push(c);
                }
            }
            
            if (line_error == 0)
            {
                size_t line_score = 0;
                while (!openings.empty())
                {
                    line_score *= 5;
                    auto cbi = std::find_if(chunks.cbegin(), chunks.cend(), [&](const auto& b) { return b.opening == openings.top(); });
                    if (cbi != chunks.cend())
                    {
                        line_score += cbi->score;
                    }
                    openings.pop();
                }
                scores.push_back(line_score);
            }
            else
            {
                total_error += line_error;
            }
        }

        set_star1_result(total_error);
        
        std::sort(scores.begin(), scores.end());
        set_star2_result(scores[(scores.size()/2)]);
    }
};
