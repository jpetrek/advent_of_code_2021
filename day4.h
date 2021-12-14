#pragma once
#include "helper.h"

class bingo_card
{
public:
    bingo_card(size_t w_, size_t h_) : w(w_), h(h_), data(h, std::vector<std::pair<long, bool>>(w, { 0,false }))
    {
        score = 0;
        order = 0;
    }

    void initialize(size_t x, size_t y, long value)
    {
        data[y][x].first = value;
    }

    void put_value(long value)
    {
        for (auto& r : data)
            for (auto& i : r)
            {
                if (i.first == value) i.second = true;
            }
    }

    bool is_winning() const
    {
        for (size_t i = 0; i < h; i++)
        {
            bool ok = true;
            for (size_t j = 0; j < w; j++)
            {
                ok = ok && data[i][j].second;
            }
            if (ok) return true;
        }

        for (size_t j = 0; j < w; j++)
        {
            bool ok = true;
            for (size_t i = 0; i < w; i++)
            {
                ok = ok && data[i][j].second;
            }
            if (ok) return true;
        }

        return false;
    }

    void set_score_and_order(size_t s, size_t o)
    {
        score = s;
        order = o;
    }

    size_t get_score() const
    {
        return score;
    }

    size_t get_order() const
    {
        return order;
    }

    long internal_score() const
    {
        long sum = 0;
        for (auto& r : data)
            for (auto& i : r)
            {
                if (!i.second) sum += i.first;
            }
        return sum;
    }

private:
    size_t w;
    size_t h;
    std::vector<std::vector<std::pair<long, bool>>> data;
    size_t score;
    size_t order;
};


class day4 : public day_base<4>
{
    void run_interal() override
    {
        std::vector<long> pickedValues;
        std::string firstLine = input_reader().get_line();
        pickedValues = helper::get_numbers_per_line<long>(firstLine, ',');
        std::vector< bingo_card > cards;

        while (!input_reader().is_end_of_file())
        {
            input_reader().get_line();
            bingo_card c(5, 5);
            for (size_t r = 0; r < 5; r++)
            {
                auto values = helper::get_numbers_per_line<long>(input_reader().get_line() , ' ');
                for (size_t i = 0; i < 5; i++)
                {
                    c.initialize(i, r, values[i]);
                }
            }
            cards.push_back(c);
        }

        size_t order = 0;
        for (auto v : pickedValues)
        {
            for (auto& b : cards)
            {
                if (!b.is_winning())
                {
                    b.put_value(v);
                    if (b.is_winning())
                    {
                        if (order == 0) set_star1_result(b.internal_score() * v);
                        b.set_score_and_order(b.internal_score() * v, order++);

                    }
                }
            }
        }

        for (auto& b : cards)
        {
            if (b.get_order() == cards.size() - 1)
            {
                set_star2_result(b.get_score());
            }
        }
    }
};
