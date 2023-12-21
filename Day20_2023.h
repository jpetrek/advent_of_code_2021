#pragma once
#include "helper.h"


struct base
{
    virtual ~base() {}
    virtual std::vector<bool> process(const bool signal, const size_t input_index, std::vector<bool>& system_state) = 0;
    std::vector<base*> inputs;
    std::vector<base*> outputs;
    std::string name;
};

struct broadcast : public base
{
    std::vector<bool> process(const bool signal, const size_t input_index, std::vector<bool>& system_state) override
    {
        if (input_index != 0) throw;
        return std::vector<bool>(outputs.size(), signal);
    }
};

struct flip_flop : public base
{
    size_t index;
    std::vector<bool> process(const bool signal, const size_t input_index, std::vector<bool>& system_state) override
    {
        if (input_index >= inputs.size()) throw;
        if (signal) return{};
        system_state[index] = !system_state[index];
        return std::vector<bool>(outputs.size(), system_state[index]);
    }
};

struct conjunction : public base
{
    std::vector<size_t> indexes;
    std::vector<bool> process(const bool signal, const size_t input_index, std::vector<bool>& system_state) override
    {
        system_state[indexes[input_index]] = signal;
        for (size_t i = indexes[0]; i <= indexes[indexes.size() - 1]; i++)
        {
            if(!system_state[i]) return std::vector<bool>(outputs.size(), true);
        }
        return std::vector<bool>(outputs.size(), false);
    }
};

struct record
{
    bool value;
    size_t input_index;
    base* target;
};

template<>
class day<20, 2023> : public day_base<20, 2023>
{   
    std::vector<bool> system_state;
    
    bool is_test() const override
    {
        return false;
    }

    void run_internal() override
    {
        std::vector<bool> system_state;
        std::vector<std::unique_ptr<base>> machine_parts;
        std::map<std::string, base*> machine_parts_ids;
        std::map<std::string, std::vector<std::string>> inputs_helper;
        std::map<std::string, std::vector<std::string>> outputs_helper;
        std::vector<conjunction*> conjunctions;
        while (!input_reader().is_end_of_file())
        {
            auto parts = helper::split(input_reader().get_line(), '-');
            if (parts[0][0] == '%')
            {
                auto p = std::make_unique<flip_flop>();
                p->name = helper::trim(parts[0].substr(1));
                machine_parts_ids[p->name] = p.get();

                p->index = system_state.size();
                system_state.push_back(false);
                
                auto outputs = helper::split(helper::trim(parts[1], 1, 0), ',');
                for (const auto o : outputs)
                {
                    helper::modify_value_in_map_safe< std::string, std::vector<std::string>>(inputs_helper, helper::trim(o), {}, [&](const auto& i) {auto r = i; r.push_back(p->name); return r; });
                    helper::modify_value_in_map_safe< std::string, std::vector<std::string>>(outputs_helper, p->name, {}, [&](const auto& i) {auto r = i; r.push_back(helper::trim(o)); return r; });
                }
                machine_parts.push_back(std::move(p));
            }
            else if (parts[0][0] == '&')
            {
                auto p = std::make_unique<conjunction>();
                conjunctions.push_back(p.get());
                p->name = helper::trim(parts[0].substr(1));
                machine_parts_ids[p->name] = p.get();
                auto outputs = helper::split(helper::trim(parts[1], 1, 0), ',');
                for (const auto o : outputs)
                {
                    helper::modify_value_in_map_safe< std::string, std::vector<std::string>>(inputs_helper, helper::trim(o), {}, [&](const auto& i) {auto r = i; r.push_back(p->name); return r; });
                    helper::modify_value_in_map_safe< std::string, std::vector<std::string>>(outputs_helper, p->name, {}, [&](const auto& i) {auto r = i; r.push_back(helper::trim(o)); return r; });
                }
                machine_parts.push_back(std::move(p));
            }
            else
            {
                auto p = std::make_unique<broadcast>();
                p->name = helper::trim(parts[0]);
                machine_parts_ids[p->name] = p.get();
                auto outputs = helper::split(helper::trim(parts[1], 1, 0), ',');
                for (const auto o : outputs)
                {
                    helper::modify_value_in_map_safe< std::string, std::vector<std::string>>(inputs_helper, helper::trim(o), {}, [&](const auto& i) {auto r = i; r.push_back(p->name); return r; });
                    helper::modify_value_in_map_safe< std::string, std::vector<std::string>>(outputs_helper, p->name, {}, [&](const auto& i) {auto r = i; r.push_back(helper::trim(o)); return r; });
                }
                machine_parts.push_back(std::move(p));
            }
        }

        for (auto& obj : machine_parts)
        {
            for (const auto& i_id : inputs_helper[obj->name])
            {
                obj->inputs.push_back(machine_parts_ids[i_id]);
            }
            for (const auto& o_id : outputs_helper[obj->name])
            {
                obj->outputs.push_back(machine_parts_ids[o_id]);
            }
        }

        for (auto* c : conjunctions)
        {
            for (auto* i : c->inputs)
            {
                c->indexes.push_back(system_state.size());
                system_state.push_back(false);
            }
        }
        
        size_t hi = 0;
        size_t lo = 0;
        std::vector<size_t> loops(4, 0);

        for (size_t pushes = 1; pushes <= 10000; pushes++)
        {
            std::vector<record> active = { {0, 0, machine_parts_ids["broadcaster"]} };
            while (active.size() > 0)
            {
                std::vector<record> new_active;
                for (auto& a : active)
                {
                    if (a.value) hi++;
                    else lo++;
                    if (a.target != nullptr)
                    {
                        auto sigs = a.target->process(a.value, a.input_index, system_state);
                        for (size_t i = 0; i < sigs.size(); i++)
                        {
                            auto* other = a.target->outputs[i];
                            if (other != nullptr)
                            {
                                for (size_t j = 0; j < other->inputs.size(); j++)
                                {
                                    if (other->inputs[j] == a.target)
                                    {
                                        new_active.push_back({ sigs[i], j, other });
                                        if ((other->name == "cn") && sigs[i])
                                        {
                                            if (loops[j] == 0) loops[j] = pushes;
                                        }
                                    }
                                }
                            }
                            else
                            {
                                new_active.push_back({ sigs[i], 0, other });
                            }
                        }
                    }
                }
                active = new_active;
            }
        }

        set_star1_result(hi * lo);
        set_star2_result(helper::lcm(loops));
    }
};

