    #pragma once
#include "helper.h"

using namespace utility::io;

template<>
class day<9, 2024> : public day_base<9,2024>
{
    struct list_item
    {
        size_t value;
        size_t size;
        bool is_space;
        bool moved;
    };

    typedef std::list<list_item> process_list;

    size_t calculate_score(const process_list& data_list)
    {
        auto li = data_list.begin();
        size_t i = 0;
        size_t sum = 0;
        while (li != data_list.end())
        {
            if (!li->is_space)
            {
                for (size_t j = 0; j < li->size; j++)
                {
                    sum += li->value * i++;
                }
            }
            else
            {
                i += li->size;
            }
            li++;
        }
        return sum;
    }

    process_list defragment_according_to_star1(process_list data_list)
    {
        auto li = data_list.begin();
        auto ri = --data_list.end();
        while (li != ri)
        {
            while ((ri->is_space || ri->size == 0) && (ri != li)) ri--;
            if ((li->is_space) && (li->size > 0) && (li != ri))
            {
                if (li->size < ri->size)
                {
                    li->value = ri->value;
                    li->is_space = false;
                    ri->size -= li->size;
                }
                else
                {
                    data_list.insert(li, *ri);
                    li->size -= ri->size;
                    ri->size = 0;
                }
            }
            else if (li != ri)
            {
                li++;
            }
        }
        return data_list;
    }

    process_list defragment_according_to_star2(process_list data_list)
    {
        auto ri = --data_list.end();
        while (ri != data_list.begin())
        {
            if ((!ri->is_space) && (!ri->moved))
            {
                ri->moved = true;
                auto li = data_list.begin();
                while (li != ri)
                {
                    if ((li->is_space) && (li->size >= ri->size))
                    {
                        data_list.insert(li, *ri);
                        li->size -= ri->size;
                        if (li->size == 0) data_list.erase(li);
                        ri->is_space = true;
                        break;
                    }
                    else
                    {
                        li++;
                    }
                }
            }
            if (ri != data_list.begin()) ri--;
        }
        return data_list;
    }

    inline bool is_test() const override { return false; }
   
    void run_internal() override
    {
        process_list data_list;
        size_t max_value = 0;
        auto input = input_reader().get_line();
        for (size_t i = 0; i < input.size(); i++)
        {
            size_t n = input[i] - '0';
            if ((i % 2) == 0)
            {
                data_list.push_back( { max_value++ , n, false, false } );
            }
            else
            {
                data_list.push_back( { n, n, true, false } );
            }
        }
        set_star1_result(calculate_score(defragment_according_to_star1(data_list)));
        set_star2_result(calculate_score(defragment_according_to_star2(data_list)));
    }   
};