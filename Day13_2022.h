#pragma once
#include "helper.h"

template<>
class day<13, 2022> : public day_base<13,2022>
{
    struct item
    {
        bool is_number = false;
        std::vector<item> list;
        int number = 0;
        bool divider = false;
    };

    void process_line_recursive(const std::string& line, size_t& actual_index, item& output)
    {
        while(actual_index < line.size())
        {
            if (line[actual_index] == '[')
            {
                actual_index++;
                output.list.push_back(item());
                process_line_recursive(line, actual_index, output.list.back());
            }
            else if (line[actual_index] == ']')
            {
                actual_index++;
                return;
            }
            else if (line[actual_index] == ',')
            {
                actual_index++;
            }
            else if ((line[actual_index] >= '0') && (line[actual_index] <= '9'))
            {
                item new_number = { true, {}, 0 };
                sscanf_s(&line[actual_index], "%d", &new_number.number);;
                actual_index+=std::to_string(new_number.number).size();
                output.list.push_back(new_number);
            }
        }
    }

    enum class compare_result
    {
        equal = 0,
        less = 1,
        greater = 2 
    };

    compare_result compare(const item& i1, const item& i2)
    {
        if ((i1.is_number) && (i2.is_number)) return i1.number == i2.number ? compare_result::equal : i1.number < i2.number ? compare_result::less : compare_result::greater;
        else if ((!i1.is_number) && (!i2.is_number))
        {
            size_t i = 0;
            while ((i < i1.list.size()) && (i < i2.list.size()))
            {
                auto r = compare(i1.list[i], i2.list[i]);
                if (r != compare_result::equal) return r;
                i++;
            }
            if ((i1.list.size() == i) && (i2.list.size() > i)) return compare_result::less;
            if ((i1.list.size() > i) && (i2.list.size() == i)) return compare_result::greater;
            return compare_result::equal;
        }
        else if ((i1.is_number) && (!i2.is_number))
        {
            item tmp_item = { false, {i1}, 0 };
            return compare(tmp_item, i2);
        }
        else if ((!i1.is_number) && (i2.is_number))
        {
            item tmp_item = { false, {i2}, 0 };
            return compare(i1, tmp_item);
        }

        throw; // :-)
    }

    item process_line(const std::string& line)
    {
        item result;
        size_t i = 0;
        process_line_recursive(line, i, result);
        return result;
    }

    
    void run_internal() override
    {
        std::vector<item> items;
        items.push_back(process_line("[[2]]"));
        items.push_back(process_line("[[6]]"));
        items[0].divider = true;
        items[1].divider = true;

        size_t right = 0;
        size_t index = 1;
        while (!input_reader().is_end_of_file())
        {
            auto it1 = process_line(input_reader().get_line());
            auto it2 = process_line(input_reader().get_line());
            items.push_back(it1);
            items.push_back(it2);
            if (compare(it1, it2) == compare_result::less) right+=index;
            input_reader().get_line();
            index++;
        }

        set_star1_result(right);

        std::sort(std::begin(items), std::end(items), [&](const auto& i1, const auto i2) {return compare(i1, i2) == compare_result::less; });

        auto c = compare(items[0], items[1]);

        size_t r = 1;
        for (size_t i = 0; i < items.size(); i++)
        {
            if (items[i].divider) r *= (i + 1);
        }

        set_star2_result(r);
    }
};