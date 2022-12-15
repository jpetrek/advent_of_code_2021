#pragma once
#include "helper.h"
#include <cctype>

template<>
class day<12, 2021> : public day_base<12, 2021>
{
    void search_path(
                    const size_t start, 
                    const size_t end, 
                    const std::map<size_t, std::vector<size_t>>& paths, 
                    const std::map<size_t, std::string>& big_caves, 
                    const std::map<size_t, std::string>& small_caves, 
                    const std::vector<size_t>& actual, 
                    std::vector< std::vector<size_t>> & discovered_paths, bool sc_already_twice
                    )
    {
        if ( actual[actual.size()-1] == end)
        {
            discovered_paths.push_back(actual);
            return;
        }

        for (const auto& p : paths.at(actual[actual.size() - 1]))
        {
            if (p != start)
            {
                size_t visited_count = std::count_if(actual.cbegin(), actual.cend(), [p](const auto i) { return i == p; });
                
                std::vector<size_t> my_actual = actual;
                my_actual.push_back(p);

                if ((big_caves.find(p) != big_caves.cend()) || (visited_count == 0))
                {
                    search_path(start, end, paths, big_caves, small_caves, my_actual, discovered_paths, sc_already_twice);
                }
                else if ((visited_count == 1) && (!sc_already_twice))
                {
                    search_path(start, end, paths, big_caves, small_caves, my_actual, discovered_paths, true);
                }
            }
        }
    }
    
    void run_internal() override
    {
        size_t index_cave = 0;

        std::map<std::string, std::size_t> big_caves_names;
        std::map<std::string, std::size_t> small_caves_names;

        std::map<size_t, std::string> big_caves;
        std::map<size_t, std::string> small_caves;
        std::map<size_t, std::vector<size_t>> connections;

        while (!input_reader().is_end_of_file())
        {
            auto conn = helper::split(input_reader().get_line(), '-');
            std::vector<size_t> indexes;

            for (const auto c : conn)
            {
                if (helper::is_all_upper(c))
                {
                    if (big_caves_names.find(c) == big_caves_names.end()) big_caves_names[c] = index_cave++;
                    big_caves[big_caves_names[c]] = c;
                    indexes.push_back(big_caves_names[c]);
                }
                else
                {
                    if (small_caves_names.find(c) == small_caves_names.end()) small_caves_names[c] = index_cave++;
                    small_caves[small_caves_names[c]] = c;
                    indexes.push_back(small_caves_names[c]);
                }
            }
    
            helper::insert_to_my_multimap(connections, indexes[0], indexes[1]);
            helper::insert_to_my_multimap(connections, indexes[1], indexes[0]);
        }

        std::vector<size_t> ap = { small_caves_names["start"] };
        std::vector<std::vector<size_t>> dp;
        search_path(small_caves_names["start"], small_caves_names["end"], connections, big_caves, small_caves, ap, dp, true);
        set_star1_result(dp.size());

        ap = { small_caves_names["start"] };
        dp = std::vector<std::vector<size_t>>();
        search_path(small_caves_names["start"], small_caves_names["end"], connections, big_caves, small_caves, ap, dp, false);
        set_star2_result(dp.size());
    }
};
