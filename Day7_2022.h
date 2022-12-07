#pragma once
#include "helper.h"

struct file
{
    size_t size;
    std::string name;
};

struct dir
{
    dir* parrent = nullptr;
    std::string name;
    std::vector<dir> subs;
    std::vector<file> files;
};

size_t calculate_dir_sizes_recursively(const dir* root, std::vector<size_t>& szs)
{
    size_t my = 0;
    for (const auto& f : root->files)
    {
        my += f.size;
    }
    for (const auto& d : root->subs)
    {
        my += calculate_dir_sizes_recursively(&d, szs);
    }
    szs.push_back(my);
    return my;
}

std::vector<size_t> calculate_dir_sizes_sorted(const dir* root)
{
    std::vector<size_t> result;
    calculate_dir_sizes_recursively(root, result);
    std::sort(result.begin(), result.end());
    return result;
}


template<>
class day<7, 2022> : public day_base<7,2022>
{
        dir create_filesystem_according_to_input()
        {
            dir root;
            root.name = "/";
            dir* current = &root;
            while (!input_reader().is_end_of_file())
            {
                auto line = input_reader().get_line();
                if (contains(line, '$'))
                {
                    auto p = helper::split(line, ' ');
                    if (p[1] == "cd")
                    {
                        if (p[2] == "/")
                        {
                            current = &root;
                        }
                        else if (p[2] == "..")
                        {
                            current = current->parrent;
                        }
                        else
                        {
                            auto it = std::find_if(current->subs.begin(), current->subs.end(), [&](const auto& md) {return md.name == p[2]; });
                            if (it != current->subs.end())
                            {
                                current = &(*it);
                            }
                        }
                    }
                }
                else
                {
                    auto p = helper::split(line, ' ');
                    if (p[0] == "dir")
                    {
                        dir tmp;
                        tmp.name = p[1];
                        tmp.parrent = current;
                        current->subs.push_back(tmp);
                    }
                    else
                    {
                        file tmp = { std::stoull(p[0]), p[1] };
                        current->files.push_back(tmp);
                    }
                }
            }
            return root;
        }
    
        void run_interal() override
        {
            auto root = create_filesystem_according_to_input();
            auto sizes = calculate_dir_sizes_sorted(&root);
            set_star1_result(std::accumulate(std::begin(sizes), std::end(sizes), size_t(0), [](auto v1, auto v2) {return v2 < 100000 ? v1 + v2 : v1; }));
            set_star2_result(*std::upper_bound(std::begin(sizes), std::end(sizes), sizes.back() - 40000000));
        }
};