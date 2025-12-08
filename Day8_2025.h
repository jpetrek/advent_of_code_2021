#pragma once
#include "helper.h"
using namespace utility::io;

template<>
class day<8, 2025> : public day_base<8,2025>
{
    typedef point_3d_generic<int64_t> position;

    struct points_with_distance
    {
        position p1;
        position p2;
        double dist;
    };

    std::vector<points_with_distance> calculate_distances_sorted(const std::vector<position>& points)
    {
        std::vector<points_with_distance> result;
        std::vector<points_with_distance> tmp;
        for (const auto& p1 : points)
            for (const auto& p2 : points)
            {
                if (!(p1 == p2))
                {
                    tmp.push_back({ p1, p2, euclides_distance(p1, p2) });
                }
            }
        std::sort(std::begin(tmp), std::end(tmp), [](const auto& d1, const auto d2) { return d1.dist < d2.dist; });

        for (size_t i = 0; i < tmp.size(); i += 2)
        {
            result.push_back(tmp[i]);
        }
        return result;
    }

    std::pair< std::map<position, size_t>, std::map<size_t, std::vector<position>>>  initialize_maps(const std::vector<position>& points)
    {
        uint64_t circuit_id = 0;
        std::map<position, size_t> point_to_circuit;
        std::map<size_t, std::vector<position>> circuit_to_points;
        for (const auto& p1 : points)
        {
            circuit_to_points[circuit_id].push_back(p1);
            point_to_circuit[p1] = circuit_id;
            circuit_id++;
        }

        return { point_to_circuit , circuit_to_points };
    }

    size_t get_multiplied_top_three_sizes(const std::map<size_t, std::vector<position>>& circuit_to_points)
    {
        std::vector<size_t> cs;
        for (const auto& c : circuit_to_points)
        {
            cs.push_back(c.second.size());
        }
        std::sort(std::begin(cs), std::end(cs), [](const auto& a, const auto& b) {return a > b; });
        return cs[0] * cs[1] * cs[2];
    }

    inline bool is_test() const override { return false; }
    void run_internal() override
    {
        std::vector<position> points;

        input_reader().read_by_line_until_condition_met_or_eof<size_t>(file_reader::no_condition(), [&](const auto& line, const auto)
            {
                auto coords = split_and_convert< int64_t>(line, ',', stoll());
                points.push_back({ coords[0], coords[1], coords[2] });
            });

        auto distances = calculate_distances_sorted(points);

        auto [point_to_circuit, circuit_to_points] = initialize_maps(points);

        for (size_t sc = 0; sc < distances.size(); sc++)
        {
            if (sc == (is_test() ? 10 : 1000))
            {
                set_star1_result(get_multiplied_top_three_sizes(circuit_to_points));
            }
            
            auto actual_connection = distances[sc];
            auto point1_id = point_to_circuit.at(actual_connection.p1);
            auto& target = circuit_to_points[point1_id];
            auto point2_id = point_to_circuit.at(actual_connection.p2);
            const auto& source = circuit_to_points[point2_id];
                
            if (point1_id != point2_id)
            {
                for (const auto& ps : source)
                {
                    target.push_back(ps);
                    point_to_circuit[ps] = point1_id;
                }
                circuit_to_points.erase(point2_id);
            }

            if (circuit_to_points.size() == 1)
            {
                set_star2_result(actual_connection.p1.x * actual_connection.p2.x);
                break;
            }
        }
    }
};