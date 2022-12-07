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

size_t count_dir_sizes(const dir* root, std::vector<size_t>& szs)
{
    size_t my = 0;
    for (const auto& f : root->files)
    {
        my += f.size;
    }
    for (const auto& d : root->subs)
    {
        my += count_dir_sizes(&d, szs);
    }
    szs.push_back(my);
    return my;
}

template<>
class day<7, 2022> : public day_base<7,2022>
{
        void run_interal() override
        {
            dir root;
            root.name = "/";
            dir* current = &root;
            auto line = input_reader().get_line();
            while (!input_reader().is_end_of_file() && !line.empty())
            {
                if (auto pos = line.find("$ cd") != std::string::npos) 
                {
                    auto p = helper::split(line, ' ');
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
                    line = input_reader().get_line();
                }
                else if (line.find("$ ls") != std::string::npos)
                {
                    line = input_reader().get_line();
                    while (!contains(line, '$') && !line.empty())
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
                        line = input_reader().get_line();
                    }
                }
            }

            std::vector<size_t> sizes;
            size_t total = count_dir_sizes(&root, sizes);
            std::sort(sizes.begin(), sizes.end());
            set_star1_result(std::accumulate(std::begin(sizes), std::end(sizes), size_t(0), [](auto v1, auto v2) {return v2 < 100000 ? v1 + v2 : v1; }));
            size_t i = 0;
            while (sizes[i] < (total - 40000000)) i++;
            set_star2_result(sizes[i]);
        }
};