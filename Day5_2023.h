#pragma once
#include "helper.h"

template<>
class day<5, 2023> : public day_base<5,2023>
{
        struct range
        {
            size_t dest;
            size_t source;
            size_t size;
        };

        size_t map(const std::vector<range>& transform, const size_t from)
        {
            for (const auto& m : transform)
            {
                if ((m.source <= from) && ((m.source + m.size) > from))
                {
                    return m.dest + (from - m.source);
                }
            }
            return from;
        }
    
        std::tuple < std::vector<range>, std::string, std::string> parse_range()
        {
            std::vector<range>result;
            auto map_line = helper::split(input_reader().get_line(), ' ');
            auto from_to = helper::split(map_line[0], '-');
            while (true)
            {
                auto l = input_reader().get_line();
                if (l.length() == 0) break;
                auto m = helper::split_convert_vector<size_t>(l, ' ', [](const auto& in) { return stoul(in); });
                result.push_back({ m[0], m[1], m[2] });
            }

            return { result, from_to[0], from_to[2] };
        }

        void run_internal() override
        {
            std::vector<std::vector<range>> ranges;
            std::map<std::string, std::string> mapping_path;
            std::map<std::string, size_t> mapped_ranges;
            
            auto seeds = helper::split_convert_vector<size_t>(helper::split(input_reader().get_line(), ':')[1], ' ', [](const auto& in) { return stoul(in); });
            input_reader().get_line();
            while (!input_reader().is_end_of_file())
            {
                auto [range, s_from, s_to] = parse_range();
                mapped_ranges[s_from] = ranges.size();
                mapping_path[s_from] = s_to;
                std::sort(std::begin(range), std::end(range), [](const auto& s1, const auto& s2) {return s1.source < s2.source; });
                ranges.push_back(range);
            }
            
            min_max_counter<size_t> star1;
            for (const auto& s : seeds)
            {
                std::string act = "seed";
                const std::string dest = "location";
                size_t number = s;
                while (act != dest)
                {
                    number = map(ranges[mapped_ranges[act]], number);
                    act = mapping_path[act];
                }
                star1.check_value(number);
            }

            set_star1_result(star1.minimum());

            min_max_counter<size_t> star2;
            for (size_t i = 0; i < seeds.size(); i += 2)
            {
                std::vector<std::pair<size_t, size_t>> buffer_ranges = { {seeds[i], seeds[i] + seeds[i + 1] -1 } };
                std::string act = "seed";
                const std::string dest = "location";
                while (act != dest)
                {
                    std::vector<std::pair<size_t, size_t>> new_ranges;
                    for (const auto r : buffer_ranges)
                    {
                        size_t l = r.first;
                        for (const auto& mr : ranges[mapped_ranges[act]])
                        {
                            if (mr.source + mr.size - 1 < l) continue;
                            if (mr.source > r.second) continue;
                            if (l < mr.source)
                            {
                                new_ranges.push_back({ map(ranges[mapped_ranges[act]], l), map(ranges[mapped_ranges[act]], mr.source - 1) });
                            }

                            l = std::max(mr.source, l);
                            auto rp = std::min(r.second, mr.source + mr.size - 1);
                            new_ranges.push_back({ map(ranges[mapped_ranges[act]], l), map(ranges[mapped_ranges[act]], rp) });
                            l = rp+1;
                        }
                        if (l < r.second) new_ranges.push_back({ map(ranges[mapped_ranges[act]], l),map(ranges[mapped_ranges[act]], r.second) });
                    }

                    buffer_ranges = new_ranges;
                    std::sort(std::begin(buffer_ranges), std::end(buffer_ranges), [](const auto& a, const auto& b) {return a.first < b.first; });

                    act = mapping_path[act];
                }

                star2.check_value(buffer_ranges[0].first);
            }
            set_star1_result(star1.minimum());
            set_star2_result(star2.minimum());
        }
};