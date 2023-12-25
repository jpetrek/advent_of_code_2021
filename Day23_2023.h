#pragma once
#include "helper.h"

template<>
class day<23, 2023> : public day_base<23, 2023>
{
    typedef point_2d_generic<size_t> point_2d;
    typedef std::tuple<std::set<point_2d>, point_2d> state;
    
    typedef std::priority_queue < state, std::vector<state>, decltype([](const state& s1, const state& s2) { return get<0>(s1).size() < get<0>(s2).size(); }) > process_queue;

    bool is_crossing(const std::map<point_2d, char >& space, const point_2d point)
    {
        bool top_hit = space.contains({ point.x, point.y - 1 }) && space.at({ point.x, point.y - 1 }) != '.';
        bool left_hit = space.contains({ point.x - 1, point.y }) && space.at({ point.x - 1, point.y }) != '.';
        bool right_hit = space.contains({ point.x + 1, point.y }) && space.at({ point.x + 1, point.y}) != '.';
        bool down_hit = space.contains({ point.x, point.y + 1 }) && space.at({ point.x, point.y + 1 }) != '.';
        size_t s = (top_hit ? 1 : 0) + (left_hit ? 1 : 0) + (right_hit ? 1 : 0) + (down_hit ? 1 : 0);
        if (s == 2) throw;
        return s > 2;
    }

    size_t find_longest_path(const point_2d start_pos, const point_2d end_pos, const std::map<point_2d, char >& space, const bool ignore_dirs)
    {
        std::map<point_2d, std::set<point_2d> > visited;
        process_queue q;
        q.push({ {},start_pos });
        min_max_counter<size_t> s1;

        while (!q.empty())
        {
            auto [my_visited, current] = q.top(); q.pop();

            if (my_visited.contains(current)) continue;
            if (visited.contains(current) && visited.at(current).size() > my_visited.size()) continue;
            my_visited.insert(current);
            visited[current] = my_visited;

            if (current == end_pos)
            {
                s1.check_value(my_visited.size() - 1);
            }

            char c = space.at(current);
            if (!ignore_dirs && c != '.')
            {
                if (c == '>') if (space.contains({ current.x + 1, current.y })) q.push({ my_visited, { current.x + 1, current.y } });
                if (c == '<') if (space.contains({ current.x - 1, current.y })) q.push({ my_visited, { current.x - 1, current.y } });
                if (c == '^') if (space.contains({ current.x, current.y - 1 })) q.push({ my_visited, { current.x, current.y - 1 } });
                if (c == 'v') if (space.contains({ current.x,current.y + 1 })) q.push({ my_visited, { current.x, current.y + 1 } });

            }
            else
            {
                if (space.contains({ current.x + 1, current.y })) q.push({ my_visited, { current.x + 1, current.y } });
                if (space.contains({ current.x - 1, current.y })) q.push({ my_visited, { current.x - 1, current.y } });
                if (space.contains({ current.x, current.y - 1 })) q.push({ my_visited, { current.x, current.y - 1 } });
                if (space.contains({ current.x,current.y + 1 })) q.push({ my_visited, { current.x, current.y + 1 } });
            }
        }
        return s1.maximum();
    }

    std::vector<std::pair< point_2d, size_t>> find_crossing(const point_2d start_pos, const std::map<point_2d, char >& space, const std::set<point_2d> crossings)
    {
        std::vector<std::pair< point_2d, size_t>> ret;
        std::map<point_2d, std::set<point_2d> > visited;
        process_queue q;
        q.push({ {},start_pos });
        min_max_counter<size_t> s1;

        while (!q.empty())
        {
            auto [my_visited, current] = q.top(); q.pop();

            if (my_visited.contains(current)) continue;
            my_visited.insert(current);
            visited[current] = my_visited;

            if (current!= start_pos && crossings.contains(current))
            {
                ret.push_back({ current,my_visited.size() });
                continue;
            }

            if (space.contains({ current.x + 1, current.y })) q.push({ my_visited, { current.x + 1, current.y } });
            if (space.contains({ current.x - 1, current.y })) q.push({ my_visited, { current.x - 1, current.y } });
            if (space.contains({ current.x, current.y - 1 })) q.push({ my_visited, { current.x, current.y - 1 } });
            if (space.contains({ current.x,current.y + 1 })) q.push({ my_visited, { current.x, current.y + 1 } });
        }

        return ret;
    }

    size_t find_point(const point_2d start_pos, const point_2d end_pos, const std::map<point_2d, char >& space, const std::set<point_2d> crossings)
    {
        std::map<point_2d, std::set<point_2d> > visited;
        process_queue q;
        q.push({ {},start_pos });
        min_max_counter<size_t> s1;

        while (!q.empty())
        {
            auto [my_visited, current] = q.top(); q.pop();

            if (my_visited.contains(current)) continue;
            my_visited.insert(current);
            visited[current] = my_visited;

            if(current != start_pos && crossings.contains(current))
            {
                continue;
            }

            if (current == end_pos)
            {
                s1.check_value(my_visited.size());
                continue;
            }

            if (space.contains({ current.x + 1, current.y })) q.push({ my_visited, { current.x + 1, current.y } });
            if (space.contains({ current.x - 1, current.y })) q.push({ my_visited, { current.x - 1, current.y } });
            if (space.contains({ current.x, current.y - 1 })) q.push({ my_visited, { current.x, current.y - 1 } });
            if (space.contains({ current.x,current.y + 1 })) q.push({ my_visited, { current.x, current.y + 1 } });
        }

        return s1.maximum();
    }

    int64_t find_longest_across_crossings(const point_2d start_crossing, const point_2d end_crossing, const std::map < point_2d, std::vector<std::pair<point_2d, size_t>>>& crossing_connections)
    {
        typedef std::tuple<std::set<point_2d>, point_2d, size_t> state1;
        typedef std::priority_queue < state1, std::vector<state1>, decltype([](const state1& s1, const state1& s2) 
            {
                double  a = static_cast<double>(get<2>(s1)) / get<0>(s1).size();
                double  b = static_cast<double>(get<2>(s2)) / get<0>(s2).size();
                return a > b; 
            }) > p_queue;

        p_queue q;
        std::map<point_2d, size_t > visited;
        q.push({ {{}},start_crossing,0 });
        while (!q.empty())
        {
            auto [my_visited, current, steps] = q.top(); q.pop();
            if (visited.contains(current) && visited.at(current) > steps) continue;
            if (my_visited.contains(current)) continue;
            my_visited.insert(current);
            visited[current] = steps;

            if (current == end_crossing && my_visited.size() == crossing_connections.size())
            {
                return steps - 1;
            }

            for (const auto& c : crossing_connections.at(current))
            {
                q.push({ my_visited, c.first, steps + c.second-1 });
            }
        }

        return 0;
    }

    bool is_test() const override
    {
        return false;
    }
    
    void run_internal() override
    {
        size_t w = 0;
        size_t h = 0;
        point_2d start_pos = { 1,0 };
        point_2d end_pos;

        std::map<point_2d, char > space;
        std::map<point_2d, std::set<point_2d> > visited;
        std::set<point_2d> crossings;


        while (!input_reader().is_end_of_file())
        {
            auto l = input_reader().get_line();
            w = l.size();
            for (size_t i = 0; i < l.size(); i++) if (l[i] != '#') space[{i, h}] = l[i];
            h++;
        }

        for (const auto& p : space)
        {
            if (is_crossing(space, p.first))
            {
                crossings.insert(p.first);
            }
        }

        end_pos = { w - 2, h - 1 };
        set_star1_result(find_longest_path(start_pos, end_pos, space, false));

        for (const auto& p : space)
        {
            if (is_crossing(space, p.first))
            {
                crossings.insert(p.first);
            }
        }

        std::pair<point_2d, size_t> start_crossing;
        std::pair<point_2d,size_t> end_crossing;

        std::map < point_2d, std::vector<std::pair<point_2d, size_t>>> crossing_connections;
        for (const auto& c : crossings)
        {
            auto s = find_point(c, start_pos, space, crossings);
            if (s > 0)
            {
                start_crossing = { c,s };
            }
            
            s = find_point(c, end_pos, space, crossings);
            if (s > 0)
            {
                end_crossing = { c,s };
            }
            crossing_connections[c] = find_crossing(c, space, crossings);
        }

        set_star2_result(start_crossing.second + end_crossing.second + find_longest_across_crossings(end_crossing.first, start_crossing.first, crossing_connections) -1 );
    }
};
