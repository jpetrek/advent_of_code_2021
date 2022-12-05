#pragma once
#include "helper.h"

template<>
class day<18, 2021> : public day_base<18, 2021>
{
    struct number_pair
    {
        number_pair(number_pair* p)
        {
            parent = p;
            left_value = -1;
            right_value = -1;
            left = nullptr;
            right = nullptr;
        }
        number_pair* parent;
        std::unique_ptr<number_pair> left;
        std::unique_ptr<number_pair> right;
        int64_t left_value;
        int64_t right_value;
    };
    
    bool split(number_pair* node)
    {
        if (node->left_value >= 10)
        {
            auto value = node->left_value;
            node->left = std::make_unique<number_pair>(node);
            node->left->left_value = value / 2;
            node->left->right_value = value / 2 + value % 2;
            node->left_value = -1;
            return true;
        }
        else if ( node->left && split(node->left.get()))
        {
            return true;
        }
        else if (node->right_value >= 10)
        {
            auto value = node->right_value;
            node->right = std::make_unique<number_pair>(node);
            node->right->left_value = value / 2;
            node->right->right_value = value / 2 + value % 2;
            node->right_value = -1;
            return true;
        }
        else
        {
            if ( node->right && split(node->right.get())) return true;
        }
        
        return false;
    }

    bool explode(number_pair* node, size_t d)
    {
        if (!node->left && !node->right && d > 4)
        {
            auto p = node;
            while ((p->parent != nullptr) && (p->parent->left.get() == p))
            {
                p = p->parent;
            }
            
            if (p->parent != nullptr)
            {
                if (!p->parent->left)
                {
                    p->parent->left_value += node->left_value;
                }
                else
                {
                    p = p->parent->left.get();
                    while ((p != nullptr) && (p->right))
                    {
                        p = p->right.get();
                    }
                    if (p != nullptr)
                    {
                        p->right_value += node->left_value;
                    }
                }
            }

            p = node;
            while ((p->parent != nullptr) && (p->parent->right.get() == p))
            {
                p = p->parent;
            }
            if (p->parent != nullptr)
            {
                if (!p->parent->right)
                {
                    p->parent->right_value += node->right_value;
                }
                else
                {
                    p = p->parent->right.get();
                    while ((p != nullptr) && (p->left))
                    {
                        p = p->left.get();
                    }
                    if (p != nullptr)
                    {
                        p->left_value += node->right_value;
                    }
                }
            }

            if (node->parent != nullptr)
            {
                auto parent = node->parent;
                if (parent->left.get() == node)
                {
                    parent->left_value = 0;
                    parent->left.reset();
                }
                else if (node->parent->right.get() == node)
                {
                    parent->right_value = 0;
                    parent->right.reset();
                }
            }
            return true;
        }
        else
        {
            if (node->left && explode(node->left.get(), d + 1)) return true;
            if (node->right && explode(node->right.get(), d + 1)) return true;
        }
        return false;
    }
    

    size_t magnitude(number_pair* node)
    {
        size_t l = node->left ? magnitude(node->left.get()) : node->left_value;
        size_t r = node->right ? magnitude(node->right.get()) : node->right_value;
        return 3 * l + 2 * r;
    }
    
    void reduce(number_pair* node)
    {
        while (explode(node, 1) || split(node));
    }

    std::unique_ptr<number_pair> create_number(const std::string& line)
    {
        std::unique_ptr<number_pair> root;
        number_pair* actual = nullptr;
        size_t lr = 0;
        for (size_t i = 0; i < line.size(); i++)
        {
            if (line[i] == '[')
            {
                if (!root.get())
                {
                    root = std::make_unique<number_pair>(nullptr);
                    actual = root.get();
                }
                else
                {
                    if ((!actual->left) && (actual->left_value == -1))
                    {
                        actual->left = std::make_unique<number_pair>(actual);
                        actual = actual->left.get();
                    }
                    else
                    {
                        actual->right = std::make_unique<number_pair>(actual);
                        actual = actual->right.get();
                    }
                }
            }
            else if (line[i] == ']')
            {
                if (actual) actual = actual->parent;
            }
            else if (line[i] == ',')
            {
            }
            else
            {
                auto c = static_cast<size_t>(line[i]) - static_cast<size_t>('0');
                if (actual)
                {
                    if ((!actual->left) && (actual->left_value == -1))
                    {
                        actual->left_value = c;
                    }
                    else
                    {
                        actual->right_value = c;
                    }
                }
            }
        }
        return root;
    }

    void run_interal() override
    {
        std::vector<std::string> input;
        input.push_back(input_reader().get_line());
        
        auto sum = create_number(input[input.size()-1]);
        reduce(sum.get());

        while (!input_reader().is_end_of_file())
        {
            input.push_back(input_reader().get_line());
            auto new_sum = std::make_unique<number_pair>(nullptr);
            sum->parent = new_sum.get();
            new_sum->left = std::move(sum);
            new_sum->right = create_number(input[input.size() - 1]);
            new_sum->right->parent = new_sum.get();
            sum = std::move(new_sum);
            reduce(sum.get());
        }

        min_max_counter<size_t> star2;
        for (const auto& s1 : input)
            for (const auto& s2 : input)
            {
                if (s1 != s2)
                {
                    auto sum = std::make_unique<number_pair>(nullptr);
                    sum->left = create_number(s1);
                    sum->right = create_number(s2);
                    sum->left.get()->parent = sum.get();
                    sum->right.get()->parent = sum.get();
                    reduce(sum.get());
                    star2.check_value(magnitude(sum.get()));
                }
            }

        set_star1_result(magnitude(sum.get()));
        set_star2_result(star2.maximum());

    }
};
