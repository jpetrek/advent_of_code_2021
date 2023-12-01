#pragma once
#include "helper.h"

template<>
class day<19, 2022> : public day_base<19, 2022>
{   
    //                 time    r_ore   r_clay  r_obs   r_geo   ore_c   clay_c  obs_c   geo_c 
    typedef std::tuple<size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t> state_key;

    struct blueprint
    {
        size_t id;
        size_t r1_ore;
        size_t r2_ore;
        size_t r3_ore;
        size_t r3_clay;
        size_t r4_ore;
        size_t r4_obs;
    };


    // for education purposes here is the recursive implementation of the same logic
    size_t analyze_recursively(const blueprint& b, state_key initial_state, std::map<state_key, size_t>& state_cache)
    {
        const size_t ore_cost_max = std::max({ b.r1_ore, b.r2_ore, b.r3_ore, b.r4_ore });
        const size_t clay_cost_max = b.r3_clay;
        const size_t obs_cost_max = b.r4_obs;

        auto [t, r_ore, r_clay, r_obs, r_geo, ore_c, clay_c, obs_c, geo_c] = initial_state;

        min_max_counter<size_t> result;
        result.check_value(geo_c);
        
        if (t > 0)
        {
                // 1. no need for more robots if already at maximum of needed production
            if (r_ore >= ore_cost_max) r_ore = ore_cost_max;
            if (r_clay >= clay_cost_max) r_clay = clay_cost_max;
            if (r_obs >= obs_cost_max) r_obs = obs_cost_max;

            // 2. when there is more than theoraticaly needed from this state to end, that we can reduce to minimum with the same result
            if (ore_c >= t * ore_cost_max - r_ore * (t - 1)) ore_c = t * ore_cost_max - r_ore * (t - 1);
            if (clay_c >= t * clay_cost_max - r_clay * (t - 1)) clay_c = t * clay_cost_max - r_clay * (t - 1);
            if (obs_c >= t * obs_cost_max - r_obs * (t - 1)) obs_c = t * obs_cost_max - r_obs * (t - 1);

            state_key optimized_actual = { t, r_ore, r_clay, r_obs, r_geo, ore_c, clay_c, obs_c, geo_c };

            if (!state_cache.contains(optimized_actual))
            {
                state_cache[optimized_actual] = 0;

                if ((ore_c >= b.r4_ore) && (obs_c >= b.r4_obs))
                {
                    result.check_value(analyze_recursively(b, { t - 1, r_ore, r_clay, r_obs , r_geo + 1, ore_c - b.r4_ore + r_ore, clay_c + r_clay, obs_c - b.r4_obs + r_obs, geo_c + r_geo }, state_cache));
                }

                if ((ore_c >= b.r3_ore) && (clay_c >= b.r3_clay))
                {
                    result.check_value(analyze_recursively(b, { t - 1, r_ore, r_clay, r_obs + 1, r_geo, ore_c - b.r3_ore + r_ore, clay_c - b.r3_clay + r_clay, obs_c + r_obs, geo_c + r_geo }, state_cache));
                }

                if (ore_c >= b.r2_ore)
                {
                    result.check_value(analyze_recursively(b, { t - 1, r_ore, r_clay + 1, r_obs, r_geo, ore_c - b.r2_ore + r_ore, clay_c + r_clay, obs_c + r_obs, geo_c + r_geo }, state_cache));
                }

                if (ore_c >= b.r1_ore)
                {
                    result.check_value(analyze_recursively(b, { t - 1, r_ore + 1, r_clay, r_obs, r_geo, ore_c - b.r1_ore + r_ore, clay_c + r_clay, obs_c + r_obs, geo_c + r_geo }, state_cache));
                }

                result.check_value(analyze_recursively(b, { t - 1, r_ore, r_clay, r_obs, r_geo, ore_c + r_ore, clay_c + r_clay, obs_c + r_obs, geo_c + r_geo }, state_cache));
            }
        }
        return result.maximum();
    }



    size_t analyze(const blueprint& b, state_key initial_state)
    {
        const size_t ore_cost_max = std::max({ b.r1_ore, b.r2_ore, b.r3_ore, b.r4_ore });
        const size_t clay_cost_max = b.r3_clay;
        const size_t obs_cost_max = b.r4_obs;

        std::map<state_key, size_t> state_cache;
        std::queue<state_key> q({ initial_state });
        min_max_counter<size_t> result;

        while (!q.empty())
        {
            auto actual_state = pop(q);
            auto [t, r_ore, r_clay, r_obs, r_geo, ore_c, clay_c, obs_c, geo_c] = actual_state;
            result.check_value(geo_c);

            if (t > 0)
            {
                // 1. no need for more robots if already at maximum of needed production
                if (r_ore >= ore_cost_max) r_ore = ore_cost_max;
                if (r_clay >= clay_cost_max) r_clay = clay_cost_max;
                if (r_obs >= obs_cost_max) r_obs = obs_cost_max;
                
                // 2. when there is more than theoraticaly needed from this state to end, that we can reduce to minimum with the same result
                if (ore_c >= t * ore_cost_max - r_ore * (t - 1)) ore_c = t * ore_cost_max - r_ore * (t - 1);
                if (clay_c >= t * clay_cost_max - r_clay * (t - 1)) clay_c = t * clay_cost_max - r_clay * (t - 1);
                if (obs_c >= t * obs_cost_max - r_obs * (t - 1)) obs_c = t * obs_cost_max - r_obs * (t - 1);

                state_key optimized_actual = { t, r_ore, r_clay, r_obs, r_geo, ore_c, clay_c, obs_c, geo_c };
                
                if (!state_cache.contains(optimized_actual))
                {
                    state_cache[optimized_actual] = 0;

                    if ((ore_c >= b.r4_ore) && (obs_c >= b.r4_obs))
                    {
                        q.push({ t - 1, r_ore, r_clay, r_obs , r_geo + 1, ore_c - b.r4_ore + r_ore, clay_c + r_clay, obs_c - b.r4_obs + r_obs, geo_c + r_geo });
                    }

                    if ((ore_c >= b.r3_ore) && (clay_c >= b.r3_clay))
                    {
                        q.push({ t - 1, r_ore, r_clay, r_obs + 1, r_geo, ore_c - b.r3_ore + r_ore, clay_c - b.r3_clay + r_clay, obs_c + r_obs, geo_c + r_geo });
                    }

                    if (ore_c >= b.r2_ore)
                    {
                        q.push({ t - 1, r_ore, r_clay + 1, r_obs, r_geo, ore_c - b.r2_ore + r_ore, clay_c + r_clay, obs_c + r_obs, geo_c + r_geo });
                    }

                    if (ore_c >= b.r1_ore)
                    {
                        q.push({ t - 1, r_ore + 1, r_clay, r_obs, r_geo, ore_c - b.r1_ore + r_ore, clay_c + r_clay, obs_c + r_obs, geo_c + r_geo });
                    }

                    q.push({ t - 1, r_ore, r_clay, r_obs, r_geo, ore_c + r_ore, clay_c + r_clay, obs_c + r_obs, geo_c + r_geo });
                }
            }
        }
        return result.maximum();
    }


    std::vector<blueprint> parse_input()
    {
        std::vector<blueprint> blueprints;
        while (!input_reader().is_end_of_file())
        {
            auto l = input_reader().get_line();
            blueprint b;
            sscanf_s(l.data(), "Blueprint %zd: Each ore robot costs %zd ore. Each clay robot costs %zd ore. Each obsidian robot costs %zd ore and %zd clay. Each geode robot costs %zd ore and %zd obsidian.",
                &b.id, &b.r1_ore, &b.r2_ore, &b.r3_ore, &b.r3_clay, &b.r4_ore, &b.r4_obs);
            blueprints.push_back(b);
        }
        return blueprints;
    }

    void run_internal() override
    {
        auto blueprints = parse_input();
        set_star1_result(std::accumulate(std::begin(blueprints), std::end(blueprints), size_t(0), [&](size_t r, auto& b) {return r + b.id * analyze(b, { 24,  1,0,0,0,0,0,0,0 }); }));
        set_star2_result(analyze(blueprints[0], { 32,  1,0,0,0,0,0,0,0 }) * analyze(blueprints[1], { 32,  1,0,0,0,0,0,0,0 }) * analyze(blueprints[2], { 32,  1,0,0,0,0,0,0,0 }));
    }
};

