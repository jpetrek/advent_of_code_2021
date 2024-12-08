#pragma once
#include "helper.h"

template<>
class day<16, 2022> : public day_base<16, 2022>
{
    struct valve
    {
        std::string id = "  ";
        long rate = 0;
        std::vector<std::string> connections;
    };

    size_t make_index_out_of_opened_set(const std::set<std::string>& opened, const std::map<std::string, size_t>& indexes)
    {
        size_t r = 0;
        for (const auto& o : opened)
        {
            r += size_t(1) << indexes.at(o);
        }
        return r;
    }

    size_t open_valve(const size_t opened, const std::string& valve, const std::map<std::string, size_t>& indexes)
    {
        return opened + (static_cast<size_t>(1) << indexes.at(valve));
    }

    bool is_valve_closed(const size_t opened, const std::string& valve, const std::map<std::string, size_t>& indexes)
    {
        size_t r = (opened >> indexes.at(valve)) & 0x1;
        return !static_cast<bool>(r);
    }

    size_t calculate_key(bool elephant, size_t time, size_t position, size_t opened)
    {
        size_t k_ele = static_cast<size_t>(elephant) << 63;
        size_t k_t = (time << 57);
        size_t k_pos = (position << 51);
        size_t k_os = opened;
        size_t key = k_ele + k_t + k_os + k_pos;
    }

    size_t calculate_rate(const std::string position, const std::map<std::string, valve>& v, const size_t time, const size_t opened, const std::map<std::string, size_t>& indexes, std::map<size_t, size_t>& hit_cache, const bool elephant)
    {
        if (time == 0)
        {
            if (elephant)
            {
                return calculate_rate("AA", v, 26, opened, indexes, hit_cache, false);
            }
            else
            {
                return 0;
            }
        }

        auto key = calculate_key(elephant, time, indexes.at(position), opened);

        if (hit_cache.contains(key)) return hit_cache.at(key);
                      
        min_max_counter<size_t> m;

        if (is_valve_closed(opened, position, indexes) && (v.at(position).rate > 0))
        {
            m.check_value((time - 1) * v.at(position).rate + calculate_rate(position, v, time - 1, open_valve(opened, position, indexes), indexes, hit_cache, elephant));
        }

        for (const auto& id : v.at(position).connections)
        {
            m.check_value(calculate_rate(id, v, time - 1, opened, indexes, hit_cache, elephant));
        }

        hit_cache[key] = m.maximum();
        return  m.maximum();
    }
    
    void run_internal() override
    {
        std::map<std::string, valve> valves;
        std::map<std::string, size_t> valves_indexes;
        size_t vi = 0;
        while (!input_reader().is_end_of_file())
        {
            auto parts =utility::string::split(input_reader().get_line(), ';');
            valve v;
            auto pos = parts[0].find("Valve ");
            v.id[0] = parts[0][pos + 6];
            v.id[1] = parts[0][pos + 7];
            sscanf_s(parts[0].data() +8, " has flow rate = %ld", &v.rate);
            pos = parts[1].find("valve");
            size_t move = (*(parts[1].data() + pos + 5)) == 's' ? 7 : 6;
            auto ids = utility::string::split(parts[1].data() + pos + move, ',');
            for (const auto& t : ids)
            {
                v.connections.push_back(utility::string::trim(t));
            }
            valves[v.id] = v;
            valves_indexes[v.id] = vi++;
        }

        std::map<size_t, size_t> hit_cache;

        set_star1_result(calculate_rate("AA", valves, 30, 0, valves_indexes, hit_cache, false));
        set_star2_result(calculate_rate("AA", valves, 26, 0, valves_indexes, hit_cache, true));
    }
};
