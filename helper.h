#pragma once
#include <cctype>
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
#include <ranges>
#include <bit>

namespace utility
{
    namespace geometry
    {
        template <typename T>
        struct point_3d_generic
        {
            T x;
            T y;
            T z;
        };

        template <typename T>
        bool operator < (const point_3d_generic<T>& p1, const point_3d_generic<T>& p2)
        {
            if (p1.x != p2.x)
            {
                return p1.x < p2.x;
            }
            if (p1.y != p2.y)
            {
                return p1.y < p2.y;
            }
            return p1.z < p2.z;
        }

        template <typename T>
        struct point_2d_generic
        {
            typedef T basic_type;
            T x;
            T y;
        };

        template <typename T>
        bool operator < (const point_2d_generic<T>& p1, const point_2d_generic<T>& p2)
        {
            if (p1.x != p2.x)
            {
                return p1.x < p2.x;
            }
            return p1.y < p2.y;
        }

        template <typename T>
        bool operator ==(const point_2d_generic<T>& p1, const point_2d_generic<T>& p2)
        {
            return ((p1.x == p2.x) && (p1.y == p2.y));
        }

        template<typename T>
        struct direction_2d_generic
        {
            struct diference
            {
                T dx;
                T dy;
            };

            enum name
            {
                n = 0,
                north = 0,
                up = 0,
                ne = 1,
                northeast = 1,
                upright = 1,
                rightup = 1,
                e = 2,
                east = 2,
                right = 2,
                se = 3,
                southeast = 3,
                rightdown = 3,
                downright = 3,
                s = 4,
                south = 4,
                down = 4,
                sw = 5,
                southwest = 5,
                leftdown = 5,
                downleft = 5,
                w = 6,
                west = 6,
                left = 6,
                nw = 7,
                northwest = 7,
                upleft = 7,
                leftup = 7
            };

            inline static const std::vector<name> four_directions = { name::n, name::s, name::e, name::w };
            inline static const std::vector<name> eight_directions = { name::n, name::s, name::e, name::w, name::ne, name::nw, name::se, name::sw };

            static diference name_to_diff(const name n) { return data[n]; }

            static std::vector<diference> generate_diferences(const std::vector<name>& names)
            {
                std::vector<diference> ret;
                for (auto d : names) ret.push_back(name_to_diff(d));
                return ret;
            }



            static void do_for_directions(const std::vector<name>& names, std::function<void(const diference dif)> action)
            {
                for (auto d : generate_diferences(names))
                {
                    action(d);
                }
            }

        private:
            inline static const std::vector<diference> data = { { 0, -1 }, { 1, -1 }, { 1, 0 }, { 1, 1 }, { 0, 1 }, { -1, 1 }, { -1, 0 } ,{ -1, -1 } };
        };

        typedef point_2d_generic<long> point_2d;
        typedef direction_2d_generic<long> direction_2d;


        template <typename T>
        point_2d_generic<T> add(const point_2d_generic<T> orig, const typename direction_2d_generic<T>::name dir)
        {
            auto d = direction_2d_generic<T>::name_to_diff(dir);
            return { orig.x + d.dx, orig.y + d.dy };
        }

        template <typename T>
        point_2d_generic<T> add(const point_2d_generic<T> orig, const typename direction_2d_generic<T>::difference d)
        {
            return { orig.x + d.dx, orig.y + d.dy };
        }


        template <typename T>
        point_2d_generic<T> sub(const point_2d_generic<T> orig, const typename direction_2d_generic<T>::name dir)
        {
            auto d = direction_2d_generic<T>::name_to_diff(dir);
            return { orig.x - d.dx, orig.y - d.dy };
        }

        template <typename T>
        point_2d_generic<T> sub(const point_2d_generic<T> orig, const typename direction_2d_generic<T>::difference d)
        {
            return { orig.x - d.dx, orig.y - d.dy };
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

        template<typename T = size_t>
        static void do_for_adjacent(T width, T height, T x, T y, const std::vector<std::pair<int, int>> diffs, std::function<void(T, T)> action)
        {
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
    };
};

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

namespace utility
{
    namespace arrays
    {
        template <typename T>
        static auto intersect(const std::set<T>& s1, const std::set<T>& s2)
        {
            std::set<T> intersection;
            for (auto value : s1)
            {
                if (s2.find(value) != s2.end())
                {
                    intersection.insert(value);
                }
            }
            return intersection;
        }

        template <typename TP, typename TA>
        bool is_outside_2D_array(const TA& a, const geometry::point_2d_generic<TP>& pos)
        {
            return (pos.x < 0) || (pos.x == a[0].size()) || (pos.y < 0) || (pos.y == a.size());
        }

        template <typename TP, typename TA>
        bool is_inside_2D_array(const TA& a, const geometry::point_2d_generic<TP>& pos)
        {
            return !is_outside_2D_array(a, pos);
        }

        template <typename TP, typename TA>
        typename TA::value_type::value_type get_value(const TA& a, const geometry::point_2d_generic<TP>& pos)
        {
            return a.at(pos.y).at(pos.x);
        }

        template <typename TP, typename TA>
        void foreach_in_2d_array(const TA& a, std::function<void(const geometry::point_2d_generic<TP>&, typename TA::value_type::value_type)> action)
        {
            for (size_t y = 0; y < a.size(); y++)
            {
                for (size_t x = 0; x < a[0].size(); x++)
                {
                    action({ static_cast<TP>(x),static_cast<TP>(y) }, a.at(y).at(x));
                }
            }
        }

        template <typename T>
        struct closest_cmp
        {
            bool operator()(const T& x, const T& y) { return x > y; }
        };

        template <typename T>
        T closest(const std::vector<T>& vec, T value)
        {
            typename std::vector<T>::const_reverse_iterator cri = std::lower_bound(vec.rbegin(), vec.rend(), value, closest_cmp<T>());
            if (cri != vec.rend())
            {
                return *cri;
            }
            return -1;
        }

        template <typename T>
        T pop(std::queue<T>& q)
        {
            auto r = q.front();
            q.pop();
            return r;
        }

        template <typename K, typename V>
        bool contains_key(const std::map<K, V>& m, K key_value)
        {
            return m.find(key_value) != std::end(m);
        }

        template <typename V>
        bool contains(const std::set<V>& m, V value)
        {
            return m.find(value) != std::end(m);
        }

        bool contains(const std::string& m, char value)
        {
            bool ret = m.find(value) != std::string::npos;
            return ret;
        }

        template <typename V>
        bool contains(const std::vector<V>& m, V value)
        {
            return std::find(std::begin(m), std::end(m), [value](const V& v1) {return v1 == value; }) != std::end(m);
        }

        template<typename T>
        static std::vector<std::vector<T>> transpose(const std::vector<std::vector<T>>& input)
        {
            std::vector<std::vector<T>> ret;
            for (size_t x = 0; x < input[0].size(); x++)
            {
                std::vector<T> line;
                for (size_t y = 0; y < input.size(); y++)
                {
                    line.push_back(input[y][x]);
                }
                ret.push_back(line);
            }
            return ret;
        }

        template<typename T>
        static std::vector<std::vector<T>> reverse_rows(const std::vector<std::vector<T>>& input)
        {
            std::vector<std::vector<T>> ret;
            for (size_t y = 0; y < input.size(); y++)
            {
                ret.push_back(input[input.size() - y - 1]);
            }
            return ret;
        }


        template<typename T>
        static std::vector<std::vector<T>> reverse_columns(const std::vector<std::vector<T>>& input)
        {
            std::vector<std::vector<T>> ret;
            for (size_t y = 0; y < input.size(); y++)
            {
                std::vector<T> line = input[y];
                std::reverse(line.begin(), line.end());
                ret.push_back(line);
            }
            return ret;
        }

        template<typename T>
        static std::vector<std::vector<T>> rotate_left(const std::vector<std::vector<T>>& input)
        {
            return reverse_rows(transpose(input));
        }

        template<typename T>
        static std::vector<std::vector<T>> rotate_right(const std::vector<std::vector<T>>& input)
        {
            return reverse_columns(transpose(input));
        }

        template<typename T>
        static void print(const std::vector<std::vector<T>>& input)
        {
            for (size_t y = 0; y < input.size(); y++)
            {
                for (size_t x = 0; x < input[0].size(); x++)
                {
                    std::cout << input[y][x];
                }
                std::cout << std::endl;
            }
        }

        template<typename T>
        static std::vector<T> slice(std::vector<T> source, size_t count)
        {
            if (count == 0) return source;
            if (count >= source.size()) return {};
            else
            {
                source.erase(source.begin() + (count - 1));
                return source;
            }
        }

        template <typename TM>
        static void modify_map(TM& map, typename TM::key_type key, typename TM::mapped_type init_value, std::function<void(typename TM::mapped_type&)> action)
        {
            if (map.find(key) == std::end(map))  map[key] = init_value;
            action(map.at(key));
        }

        template <typename TK, typename TV>
        static void modify_value_in_map_safe(std::map<TK, TV>& map, TK key, TV init_value, std::function<TV(const TV&)> action)
        {
            if (map.find(key) == std::end(map))  map[key] = init_value;
            map.at(key) = action(map.at(key));
        }


        template<typename tK, typename tV>
        static void insert_to_my_multimap(std::map<tK, std::vector<tV>>& my_multimap, tK key, tV value)
        {
            if (my_multimap.find(key) == std::end(my_multimap)) my_multimap[key] = std::vector<tV>();
            my_multimap[key].push_back(value);
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

        static std::vector<std::string> extract_from_input_according_to_positions(const std::string& input, std::vector<std::pair<size_t, size_t>> positions)
        {
            std::vector<std::string> result;
            for (const auto& pos : positions)
            {
                result.push_back(input.substr(pos.first, pos.second - pos.first + 1));
            }
            return result;
        }

        template <typename T>
        static std::vector<T> set_to_vector(const std::set<T> s)
        {
            std::vector<T> ret;
            for (const auto& si : s) ret.push_back(si);
            return ret;
        }

        template <typename T, std::size_t... Indices>
        static auto vector_to_tuple_helper(const std::vector<T>& v, std::index_sequence<Indices...>) {
            return std::make_tuple(v[Indices]...);
        }

        template <std::size_t N, typename T>
        static auto vector_to_tuple(const std::vector<T>& v) {
            return vector_to_tuple_helper(v, std::make_index_sequence<N>());
        }

        template<typename V, typename T>
        static std::set<V> extract_if(const T& in, std::function<bool(const typename  T::value_type&)> cond, std::function<V(const typename T::value_type&)> ex)
        {
            std::set<V> ret;
            for (const auto& v : in)
            {
                if (cond(v)) ret.insert(ex(v));
            }
            return ret;
        }

        template <typename R, typename T>
        static R accumulate_if(const T& data, R init, std::function < R(const size_t, const typename T::value_type&)> acc)
        {
            R ret = init;
            for (size_t i = 0; i < data.size(); i++)
            {
                ret += acc(i, data.at(i));
            }
            return ret;
        }

        template <typename R, typename T>
        static R accumulate_generic(const T& data, R init, std::function < R(const R, const typename T::value_type&)> acc)
        {
            R ret = init;
            for (auto& i : data)
            {
                ret = acc(ret, i);
            }

            return ret;
        }

        template <typename T>
        static std::pair<T, T> vector_to_pair(const std::vector<T>& v)
        {
            std::pair<T, T> ret;
            std::tie(ret.first, ret.second) = vector_to_tuple<2, T>(v);
            return ret;
        }
    };
};

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

namespace utility
{
    namespace math
    {
        template <typename T>
        T pow10(T how)
        {
            return static_cast<T>(pow(10, how));
        }


        template <typename T>
        static std::function<bool(const T&)> is_zero()
        {
            return [](const auto& i) {return i == 0; };
        }

        template <typename T>
        static std::function<bool(const T&)> is_bigger()
        {
            return [](const auto& a, const auto& b) {return a > b; };
        }

        template <typename T>
        static std::function<bool(const T&)> is_smaller()
        {
            return [](const auto& a, const auto& b) {return a < b; };
        }
        
        template <typename T>
        static std::pair<T, T> get_min_max(T v1, T v2)
        {
            if (v1 < v2) return { v1,v2 };
            else return { v2, v1 };
        }
        
        template <typename T>
        T digits10(uint64_t v)
        {
            return  1
                + (T)(v >= 10)
                + (T)(v >= 100)
                + (T)(v >= 1000)
                + (T)(v >= 10000)
                + (T)(v >= 100000)
                + (T)(v >= 1000000)
                + (T)(v >= 10000000)
                + (T)(v >= 100000000)
                + (T)(v >= 1000000000)
                + (T)(v >= 10000000000ull)
                + (T)(v >= 100000000000ull)
                + (T)(v >= 1000000000000ull)
                + (T)(v >= 10000000000000ull)
                + (T)(v >= 100000000000000ull)
                + (T)(v >= 1000000000000000ull)
                + (T)(v >= 10000000000000000ull)
                + (T)(v >= 100000000000000000ull)
                + (T)(v >= 1000000000000000000ull)
                + (T)(v >= 10000000000000000000ull);
        }

        template <typename T>
        T sign(const T val)
        {
            if (val > 0) return 1;
            if (val < 0) return -1;
            return 0;
        }

        template<typename T, typename TAverage>
        struct min_max_summary_average_counter
        {
            min_max_summary_average_counter() : value_maximum(std::numeric_limits<T>::min()), value_minimum(std::numeric_limits<T>::max()), value_sum(T()), value_count(0)
            {
            }

            inline void check_value(const T value)
            {
                if (value_maximum < value) value_maximum = value;
                if (value_minimum > value) value_minimum = value;
                value_count++;
                value_sum += value;
            }

            T minimum() const
            {
                return value_minimum;
            }

            T maximum() const
            {
                return value_maximum;
            }

            T summary() const
            {
                return value_sum;
            }

            TAverage average() const
            {
                return static_cast<TAverage>(value_sum) / value_count;
            }

            T value_minimum;
            T value_maximum;
            T value_sum;
            size_t value_count;
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

            void adjust_limits(T minimum, T maximum)
            {
                value_minimum = minimum;
                value_maximum = maximum;
            }

            T value_minimum;
            T value_maximum;
        };

        template<typename T>
        min_max_counter<T> modify_limits_symmetrically(const min_max_counter<T>& limits, T modificator)
        {
            min_max_counter<T> ret = limits;
            ret.check_value(limits.minimum() - modificator);
            ret.check_value(limits.maximum() + modificator);
            return ret;
        }

        template<typename T>
        struct summary_counter
        {
            summary_counter(const T init = 0) : val(init) {}

            inline void add(const T v)
            {
                val += v;
            }

            T value() const
            {
                return val;
            }

        private:
            T val;
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

        static size_t lcm(const std::vector<size_t> in)
        {
            size_t r = in[0];
            for (size_t i = 1; i < in.size(); i++)
            {
                r = std::lcm(r, in[i]);
            }
            return r;
        }

        static std::vector<double> solve_quadratic_eq(const double a, const double b, const double c)
        {
            double discriminant = b * b - 4.0 * a * c;
            if (discriminant < 0) return {};
            if (discriminant == 0) return { b / (2 * a) };
            double r1 = (b + sqrt(discriminant)) / (2 * a);
            double r2 = (b - sqrt(discriminant)) / (2 * a);
            return { std::min(r1,r2), std::max(r1,r2) };
        }

        typedef struct
        {
            double x;
            double y;
            bool has_result;
        } two_unknowns_result;
        
        
        static two_unknowns_result solve_2unknowns(const double a, const double b, const double e, const double c, const double d, const double f)
        {
            // ax + by = e
            // cx + dy = f
            double determinant = a * d - b * c;
            if (determinant == 0) return { 0,0,false };
            double x = (e * d - b * f) / determinant;
            double y = (a * f - e * c) / determinant;
            return { x,y, true };
        }
    };
};

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

namespace utility
{
    namespace string
    {
        static std::string slice(const std::string& source, size_t count)
        {
            if (count == 0) return source;
            if (count >= source.size()) return "";
            else
            {
                return source.substr(count);
            }
        }

        static std::function<unsigned long long(const std::string&)> stoull()
        {
            return [](const auto& i) {return std::stoull(i); };
        }

        static std::function<unsigned long(const std::string&)> stoul()
        {
            return [](const auto& i) {return std::stoul(i); };
        }

        static std::function<long long(const std::string&)> stoll()
        {
            return [](const auto& i) {return std::stoll(i); };
        }

        static std::function<long(const std::string&)> stol()
        {
            return [](const auto& i) {return std::stol(i); };
        }

        static std::string trim(const std::string& s)
        {
            std::string ret;
            for (auto c : s)
            {
                if (c != ' ') ret.push_back(c);
            }
            return ret;
        }

        static std::string trim(const std::string& s, const size_t tl, const size_t tr)
        {
            return s.substr(tl, s.size() - tl - tr);
        }

        static std::pair<std::string, std::string> split_to_two(const std::string& s, const char c)
        {
            auto i = s.find(c);
            if (i == std::string::npos) throw;
            return { s.substr(0, i) , s.substr(i) };
        }

        static bool is_all_upper(const std::string& i)
        {
            return std::all_of(i.cbegin(), i.cend(), [](const auto c) { return std::isupper(c); });
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

        static std::vector<std::pair<std::string, std::string>> double_split(const std::string& s, char delim1, char delim2)
        {
            std::vector<std::pair<std::string, std::string>> result;
            auto s1 = split(s, delim1);
            for (const auto& a : s1)
            {
                auto r = split(a, delim2);
                if (r.size() == 1) result.push_back({ r[0], "" });
                if (r.size() == 2) result.push_back({ r[0], r[1] });
            }
            return result;
        }

        template<typename T>
        static std::vector<T> split_and_convert(const std::string& s, const char delim, std::function<T(const std::string&)> convert = [](const std::string& n)
            {
                std::stringstream conv(n);
                T a;
                conv >> a;
                return a;
            })
        {
            auto parts = split(s, delim);
            std::vector<T> result;
            for (auto& p : parts)
            {
                result.push_back(convert(p));
            }
            return result;
        }

        template<typename T>
        static T split_and_convert_generic(const std::string& s, const char delim, std::function<void(T&, const std::string&)> store)
        {
            T result;
            auto parts = split(s, delim);
            for (auto& p : parts)
            {
                store(result, p);
            }
            return result;
        }

        template<typename T>
        static std::set<T> split_convert_set(const std::string& s, const char delim, std::function<T(const std::string&)> conv)
        {
            std::set <T> result;
            auto parts = split(s, delim);
            for (auto& p : parts)
            {
                result.insert(conv(p));
            }
            return result;
        }

        template<typename T>
        static std::vector<T> split_convert_vector(const std::string& s, const char delim, std::function<T(const std::string&)> conv)
        {
            std::vector<T> result;
            auto parts = split(s, delim);
            for (auto& p : parts)
            {
                result.push_back(conv(p));
            }
            return result;
        }

        template <typename T>
        static std::vector<T> convert_string_of_digits_to_vector(const std::string& line)
        {
            std::vector<T> ret;
            for (auto c : line)
            {
                ret.push_back(static_cast<size_t>(c) - static_cast<size_t>('0'));
            }
            return ret;
        }
    };
};



//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
namespace utility
{
    namespace io
    {
        struct file_reader
        {
            file_reader(const std::string& _file_name) : file_name(_file_name), stream(_file_name)
            {
            }
            ~file_reader()
            {
                stream.close();
            }
            struct read_conditions
            {
                struct nocondition {};
                struct empty_line {};
            };

            template<typename T = read_conditions::nocondition>
            void read_by_line_until_condition_met_or_eof(std::function<void(const std::string&)> action)
            {
                while (!is_end_of_file())
                {
                    std::string line = get_line();
                    action(line);
                }
            };

            template<>
            void read_by_line_until_condition_met_or_eof<read_conditions::empty_line>(std::function<void(const std::string&)> action)
            {
                while (!is_end_of_file())
                {
                    std::string line = get_line();
                    if (line == "") break;
                    action(line);
                }
            }

            std::string read_file()
            {
                std::string r;
                while (!is_end_of_file())
                {
                    r += get_line();
                }
                return r;
            }

            std::string get_line()
            {
                std::string line;
                std::getline(stream, line);
                return line;
            }

            void skip_line()
            {
                get_line();
            }

            std::string get_file_name() const
            {
                return file_name;
            }

            bool is_opened() const
            {
                return !stream.fail();
            }

            bool is_end_of_file() const
            {
                return !is_opened() || stream.eof();
            }

        private:
            std::string file_name;
            std::ifstream stream;
        };
        
        
        template<typename T>
        static std::vector<std::vector<T>> transform_input_into_array(file_reader& input_reader, std::function<T(char)> tr)
        {
            std::vector<std::vector<T>> data;
            while (!input_reader.is_end_of_file())
            {
                auto line = input_reader.get_line();
                if (line.size() == 0) return data;
                std::vector<T> lv;
                for (auto c : line)
                {
                    lv.push_back(tr(c));
                }
                data.push_back(lv);
            }
            return data;
        }

        static std::vector<std::vector<char>> transform_input_into_char_array(file_reader& input_reader)
        {
            return transform_input_into_array<char>(input_reader, [](const char c) {return c; });
        }
    };

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    namespace aoc
    {
        template<size_t D, size_t Y>
        struct day_base
        {
            day_base() : reader(nullptr), is_s1_set(false), is_s2_set(false)
            {
            }

            void run()
            {
                if (is_test())
                {
                    reader = make_unique<utility::io::file_reader>(PATH_TO_TEST_DATA(Y, D));
                }
                else
                {
                    reader = make_unique<utility::io::file_reader>(PATH_TO_DATA(Y, D));
                }

                log_header();
                run_internal();
                log_star1();
                log_star2();
                log_footer();
            }

        protected:

            virtual void run_internal() = 0;

            inline virtual bool is_test() const { return false; }

            utility::io::file_reader& input_reader()
            {
                return *reader;
            }

            template<typename T>
            void set_star1_result(T v)
            {
                set_star1_result(std::to_string(v));
            }

            template<typename T>
            void set_star2_result(T v)
            {
                set_star2_result(std::to_string(v));
            }

            void set_star1_result(std::string v)
            {
                is_s1_set = true;
                s1_value = v;
            }

            void set_star2_result(std::string v)
            {
                is_s2_set = true;
                s2_value = v;
            }

        private:

            void log_header()
            {
                std::cout << "AOC " << Y << " - Day " << D << std::endl;
                if (is_test()) std::cout << "Test input used!" << std::endl;
                if (reader->is_opened()) std::cout << "File: '" << reader->get_file_name() << "' found!" << std::endl;
                else std::cout << "File: '" << reader->get_file_name() << "' NOT found!" << std::endl;
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

            std::unique_ptr<utility::io::file_reader> reader;
            std::string s1_value;
            bool is_s1_set;
            std::string s2_value;
            bool is_s2_set;
        };

        template<size_t D, size_t R>
        class day : public day_base<D, R>
        {
        };

        template <class T>
        static void run_day()
        {
            T day;
            day.run();
        }

        template <std::size_t Y, std::size_t Begin, std::size_t... Indices>
        static void _run(std::index_sequence<Indices...>)
        {
            return (run_day<day<Begin + Indices + 1, Y>>(), ...);
        }

        template <std::size_t Y, std::size_t Mbegin, std::size_t MEnd>
        static void run()
        {
            _run<Y, Mbegin - 1>(std::make_index_sequence<MEnd - (Mbegin - 1)>());
        }
    };
};

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

using namespace utility::aoc;

