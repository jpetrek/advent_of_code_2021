#pragma once
#include "helper.h"
using namespace utility::arrays;
using namespace utility::string;
using namespace utility::math;

template<>
class day<11, 2025> : public day_base<11,2025>
{
    inline bool is_test() const override { return false; }

    template<typename T>
    size_t count_paths_with_required_nodes(const graph_weighted_map<T>& graph, size_t s, size_t t, const std::vector<size_t>& required)
    {
        std::unordered_map<size_t, int> node_id_to_req_index;
        for (int i = 0; i < required.size(); ++i)
        {
            node_id_to_req_index[required[i]] = i;
        }

        std::vector<size_t> topological_order = topological_sort(graph);

        std::unordered_map<size_t, std::vector<size_t>> ways;
        for (const auto u : graph.nodes())
        {
            ways[u] = std::vector<size_t>(bool_storage::number_of_combinations(required.size()), 0);
        }

        uint8_t start_mask = 0;
        if (auto it = node_id_to_req_index.find(s); it != node_id_to_req_index.end())
        {
            bool_storage::set_variable(start_mask, it->second);
        }

        ways[s][start_mask] = 1;

        for (size_t u : topological_order)
        {
            for (uint8_t m = 0; m < bool_storage::number_of_combinations(required.size()); ++m)
            {
                size_t current_count = ways[u][m];
                if (current_count != 0)
                {
                    for (auto v : graph.get_adjacend(u))
                    {
                        uint8_t updated_mask = m;
                        if (auto node_id_req_index = node_id_to_req_index.find(v.first); node_id_req_index != node_id_to_req_index.end())
                        {
                            bool_storage::set_variable(updated_mask, node_id_req_index->second);
                        }
                        ways[v.first][updated_mask] += current_count;
                    }
                }
            }
        }

        return ways[t][bool_storage::full_mask<uint8_t>(required.size())];
    }

    void run_internal() override
    {
        std::map<std::string, size_t> name_to_node_id;
        graph_weighted_map g;
        size_t node_id = 0;

        input_reader().read_by_line_until_condition_met_or_eof<size_t>(file_reader::no_condition(), [&](const auto& line, const auto)
            {
                auto s1 = split(line, ':');
                if (!name_to_node_id.contains(s1[0])) name_to_node_id[s1[0]] = node_id++;
                size_t id = name_to_node_id.at(s1[0]);

                auto s2 = split(s1[1], ' ');
                for (const auto& s : s2)
                {
                    if (!name_to_node_id.contains(s)) name_to_node_id[s] = node_id++;
                    g.add_onedirectional_edge(id, name_to_node_id.at(s),0);
                }
            });

        set_star1_result(count_paths_with_required_nodes(g, name_to_node_id.at("you"), name_to_node_id.at("out"), {}));
        set_star2_result(count_paths_with_required_nodes(g, name_to_node_id.at("svr"), name_to_node_id.at("out"), { name_to_node_id.at("dac") ,name_to_node_id.at("fft") }));
    }
};