#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <utility>
#include <numeric>
#include <functional>
#include <limits>
#include <algorithm>
#include <map>
#include <set>
#include <stack>
#include <queue>

struct file_reader
{
    file_reader(const std::string& file_name) : stream(file_name)
    {
    }
    ~file_reader()
    {
        stream.close();
    }

    std::string get_line()
    {
        std::string line;
        std::getline(stream, line);
        return line;
    }

    bool is_end_of_file() const
    {
        return stream.eof();
    }

private:
    std::ifstream stream;
};

template<size_t D>
struct day_base
{
    day_base() : reader("d:\\i" + std::to_string(D) + ".txt"), day(D), is_s1_set(false), is_s2_set(false)
    {
    }

    void run()
    {
        log_header();
        run_interal();
        log_star1();
        log_star2();
        log_footer();
    }
    
protected:
    
    virtual void run_interal() = 0;
    
    file_reader& input_reader()
    {
        return reader;
    }

    void set_star1_result(size_t v)
    {
        is_s1_set = true;
        s1_value = v;
    }

    void set_star2_result(size_t v)
    {
        is_s2_set = true;
        s2_value = v;
    }

private:

    void log_header()
    {
        std::cout << "AOC 2021 - Day " << day << std::endl;
    }

    void log_footer()
    {
        std::cout << "-------------------" << std::endl;
    }

    void log_star1()
    {
        std::cout << "Star 1: ";
        if (is_s1_set) std::cout << s1_value << std::endl;
        else std::cout << "not implemented yet" << std::endl;
    }

    void log_star2()
    {
        std::cout << "Star 2: ";
        if (is_s2_set) std::cout << s2_value << std::endl;
        else std::cout << "not implemented yet" << std::endl;
    }

    file_reader reader;
    size_t day;
    size_t s1_value;
    bool is_s1_set;
    size_t s2_value;
    bool is_s2_set;
};

template<typename T>
struct min_max_counter
{
    min_max_counter() : value_maximum(std::numeric_limits<T>::min()), value_minimum(std::numeric_limits<T>::max())
    {
    }

    inline void check_value(const T value)
    {
        if (value_maximum < value) value_maximum = value;
        if (value_minimum > value) value_minimum = value;
    }

    T minimum() const
    {
        return value_minimum;
    }

    T maximum() const
    {
        return value_maximum;
    }

    T value_minimum;
    T value_maximum;
};

struct graph_weighted
{
    graph_weighted(size_t v_count) : vertex_count(v_count), adjacents(v_count, std::vector<std::pair<int64_t, int64_t>>())
    {
    }

    size_t count() const
    {
        return vertex_count;
    }

    void add_twodirectional_edge(size_t v1, size_t v2, int64_t w1, int64_t w2)
    {
        add_onedirectional_edge(v1, v2, w1);
        add_onedirectional_edge(v2, v1, w2);
    }

    void add_onedirectional_edge(size_t v1, size_t v2, int64_t w1)
    {
        auto i = std::find_if(std::begin(adjacents[v1]), std::end(adjacents[v1]), [v2](const auto& p) {return p.first == v2; });
        if (i == std::end(adjacents[v1]))
        {
            adjacents[v1].push_back({ v2, w1 });
        }
    }

    const std::vector<std::pair<int64_t, int64_t>>& get_adjacend(size_t vertex) const
    {
        return adjacents[vertex];
    }

private:
    size_t vertex_count;
    std::vector<std::vector<std::pair<int64_t, int64_t> >> adjacents;
};

struct helper
{
    static size_t dijkstra_shortest_path(const graph_weighted& graph, size_t src, size_t dest)
    {
        std::priority_queue<std::pair<int64_t, int64_t>, std::vector<std::pair<int64_t, int64_t>>, std::greater<std::pair<int64_t, int64_t>> > pq;
        std::vector<int64_t> dist(graph.count(), std::numeric_limits<int64_t>::max());
        pq.push({ 0, src });
        dist[src] = 0;
        while (!pq.empty())
        {
            int64_t u = pq.top().second;
            pq.pop();
            for (auto x : graph.get_adjacend(u))
            {
                auto v = x.first;
                auto  weight = x.second;
                if (dist[v] > dist[u] + weight)
                {
                    dist[v] = dist[u] + weight;
                    pq.push({ dist[v], v });
                }
            }
        }
        return dist[dest];
    }

    template <typename TK, typename TV>
    static void modify_value_in_map_safe(std::map<TK, TV>& map, TK key, TV init_value, std::function<void(TV&)> action)
    {
        if (map.find(key) == std::end(map))  map[key] = init_value;
        action(map.at(key));
    }
    
    
    static bool is_all_upper(const std::string& i)
    {
        return std::all_of(i.cbegin(), i.cend(), [](const auto c) { return std::isupper(c); });
    }

    template<typename tK, typename tV>
    static void insert_to_my_multimap(std::map<tK, std::vector<tV>>& my_multimap, tK key, tV value)
    {
        if (my_multimap.find(key) == std::end(my_multimap)) my_multimap[key] = std::vector<tV>();
        my_multimap[key].push_back(value);
    }

    static void do_for_adjacent(size_t width, size_t height, size_t x, size_t y, std::function<void(size_t, size_t)> action)
    {
        const std::vector<std::pair<int, int>> diffs = { {-1, -1}, {-1, 0}, {-1, 1}, {0,-1}, {0,1}, {1, -1}, {1, 0}, {1, 1} };
        for (const auto& diff : diffs)
        {
            long n_x = static_cast<long>(x) + diff.first;
            long n_y = static_cast<long>(y) + diff.second;
            if ((n_x >= 0) && (n_x < width) && (n_y >= 0) && (n_y < height))
            {
                action(n_x, n_y);
            }
        }
    }
    
    
    template<typename T, typename T1 = T>
    static T get_value_safe(const std::vector<std::vector<T>> data, size_t x, size_t y, int diff_x, int diff_y, T1 default_value)
    {
        int new_x = static_cast<int>(x) + diff_x;
        int new_y = static_cast<int>(y) + diff_y;

        if ((new_x < 0) || (new_y < 0)) return default_value;
        if ((new_x >= data[0].size()) || (new_y >= data.size())) return default_value;
        
        return data[new_y][new_x];
    }


    
    static bool strings_are_similar(const std::string& first, const std::string& second)
    {
        if (first.size() != second.size()) return false;
        for (size_t i = 0; i < first.size(); i++)
        {
            if (second.find(first[i]) == std::string::npos)
            {
                return false;
            }
        }
        return true;
    }
    
    static bool permute(std::string s, std::string answer, std::function<bool(const std::string&)> use_string)
    {
        if (s.length() == 0)
        {
            return use_string(answer);
        }
        for (size_t i = 0; i < s.length(); i++)
        {
            char ch = s[i];
            std::string left_substr = s.substr(0, i);
            std::string right_substr = s.substr(i + 1);
            std::string rest = left_substr + right_substr;
            if (permute(rest, answer + ch, use_string))
            {
                return true;
            }
        }
        return false;
    }
    
    static std::vector<std::string> split(const std::string& s, char delim) 
    {
        std::vector<std::string> result;
        std::stringstream ss(s);
        std::string item;
        while (getline(ss, item, delim)) 
        {
            if (item.size() > 0) result.push_back(item);
        }
        return result;
    }
    
    template <typename T>
    static std::vector<T> get_numbers_per_line(const std::string& line)
    {
        std::vector<T> ret;
        for (auto c : line)
        {
            ret.push_back(static_cast<size_t>(c) - static_cast<size_t>('0'));
        }
        return ret;
    }

    template<typename T>
    static std::vector<T> get_numbers_per_line(const std::string& line, char delim)
    {
        std::vector<T> ret;
        auto numbers = split(line, delim);
        for (const auto& n : numbers)
        {
            if (n.size() > 0)
            {
                std::stringstream conv(n);
                T a;
                conv >> a;
                ret.push_back(a);
            }
        }
        return ret;
    }

};