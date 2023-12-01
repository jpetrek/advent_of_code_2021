#pragma once
#include "helper.h"

struct dl_list_item
{
    int64_t value;
    dl_list_item* previous = nullptr;
    dl_list_item* next = nullptr;
};

template<>
class day<20, 2022> : public day_base<20, 2022>
{   
    std::vector<std::unique_ptr<dl_list_item>> storage;
    dl_list_item* head = nullptr;
    dl_list_item* zero = nullptr;

    void run_internal() override
    {
        size_t i = 0;
        while (!input_reader().is_end_of_file())
        {
            int64_t n = std::stoll(input_reader().get_line());
            storage.push_back(std::make_unique<dl_list_item>());
            storage.back()->value = n * 811589153;
//S1          storage.back()->value = n;

            if (storage.back()->value == 0) zero = storage.back().get();
            if (storage.size() > 1)
            {
                storage.back()->previous = storage[storage.size() - 2].get();
                storage[storage.size() - 2]->next = storage.back().get();
            }
            else
            {
                head = storage.back().get();
            }
        }
        storage.back()->next = storage.front().get();
        storage.front()->previous = storage.back().get();

        for (size_t j = 0; j < 10 /*S1: 1*/; j++)
        {
            for (auto& i : storage)
            {
                size_t ns = abs(i->value) % (storage.size() - 1);
                auto current = i->value > 0 ? i->next : i->previous;
                i.get()->previous->next = i.get()->next;
                i.get()->next->previous = i.get()->previous;
                size_t c = 1;
                if (i->value < 0)
                {
                    while (c <= ns)
                    {
                        current = current->previous;
                        c++;
                    }
                }
                else
                {
                    while (c < ns)
                    {
                        current = current->next;
                        c++;
                    }
                }
                if (current == i.get())
                {
                    std::cout << "hit!" << std::endl;
                }
                else
                {
                    current->next->previous = i.get();
                    i.get()->next = current->next;
                    i->previous = current;
                    current->next = i.get();
                }
            }
        }

        int64_t s1 = 0;
        auto current = zero;
        for (size_t i = 0; i < 3001; i++)
        {
            if (i == 1000) s1 += current->value;
            if (i == 2000) s1 += current->value;
            if (i == 3000) s1 += current->value;
            current = current->next;
        }
        set_star1_result(s1);
    }
};

