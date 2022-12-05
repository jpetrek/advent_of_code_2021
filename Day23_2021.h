#pragma once
#include "helper.h"

template<>
class day<23, 2021> : public day_base<23, 2021>
{

    class amphipod
    {
    public:
        enum amphipod_type
        {
            SHIT = 0,
            A = 1,
            B = 10,
            C = 100,
            D = 1000
        };

        enum amphipod_state
        {
            uknown,
            start,
            after_first_move,
            finish
        };

        amphipod(std::string n , amphipod_type t) : name(n), type(t), state(amphipod_state::uknown), score(0), position(-1, -1)
        {
        }

        void place(int64_t x, int64_t y)
        {
            position.first = x;
            position.second = y;
            if (state == uknown) state = start;
            else if (state == start) state = after_first_move;
            else state = finish;
        }

        std::string get_name()const
        {
            return name;
        }


        amphipod_state get_state()const
        {
            return state;
        }

        amphipod_type get_type() const
        {
            return type;
        }

        size_t get_score()const
        {
            return score;
        }

        std::pair<int64_t, int64_t> get_position() const
        {
            return position;
        }

        amphipod_type type;
        amphipod_state state;
        size_t score;
        std::pair<int64_t, int64_t> position;
        std::string name;
    };

    struct move
    {
        size_t t_x = 0;
        size_t t_y = 0;
        size_t number_of_steps = 0;
    };

    // preferred path

    move find_path_home(size_t x, size_t y, amphipod::amphipod_type t, const std::vector<std::vector<int64_t>>& desk, const std::vector<amphipod>& amnipods)
    {
        if (t == amphipod::amphipod_type::SHIT) return  { 0,0,0 };
        
        move ret;
        for (int64_t m_y = y-1; m_y >=0; m_y--)
        {
            ret.number_of_steps++;
            if (desk[m_y][x] != 0) return { 0,0,0 };
        }

        size_t target_x;
        if (t == amphipod::amphipod_type::A) target_x = 2;
        if (t == amphipod::amphipod_type::B) target_x = 4;
        if (t == amphipod::amphipod_type::C) target_x = 6;
        if (t == amphipod::amphipod_type::D) target_x = 8;

        size_t m_x = x;
        int64_t dx = target_x < x ? -1 : 1;
        while (m_x != target_x)
        {
            ret.number_of_steps++;
            if (desk[0][x] != 0) return { 0,0,0 };
            m_x += dx;
        }

        for (size_t m_y = 0; m_y < 5; m_y++)
        {
            if (desk[m_y][target_x] != 0)
            {
                if (amnipods[desk[m_y][target_x]].get_type() != t) return {0,0,0};
            }
        }

        if (desk[1][target_x] != 0) return {0,0,0};

        size_t m_y = 0;
        while ((m_y < 5) && (desk[m_y][target_x] == 0))
        {
            ret.number_of_steps++;
            m_y++;
        }
        
        ret.t_x = target_x;
        ret.t_y = m_y - 1;
        ret.number_of_steps--;
        return ret;
    }


    
    
    std::vector<move> find_all_moves(const amphipod& a, const std::vector<std::vector<int64_t>>& desk, const std::vector<amphipod>& amnipods)
    {
        std::vector<move> moves;
        auto p = a.get_position();
        
        if (a.get_state() != amphipod::amphipod_state::finish)
        {
            auto ph = find_path_home(p.first, p.second, a.get_type(), desk, amnipods);
            if (ph.number_of_steps > 0)
            {
                moves.push_back(ph);
            }
        }

        if (a.get_state() == amphipod::amphipod_state::start)
        {
            size_t ns = 0;
            bool can_go = true;
            for (int64_t m_y = p.second - 1; m_y >=0; m_y--)
            {
                can_go = can_go && desk[m_y][p.first] == 0;
                ns++;
            }
            if (can_go)
            {
                int64_t actual_x = p.first;
                size_t m_ns = ns;
                while ((actual_x >= 0) && (desk[0][actual_x] == 0))
                {
                    if ((actual_x != 2) && (actual_x != 4) && (actual_x != 6) && (actual_x != 8))
                    {
                        moves.push_back({ static_cast<size_t>(actual_x), 0, m_ns });
                    }
                    m_ns++;
                    actual_x--;
                }

                actual_x = p.first;
                m_ns = ns;
                while ((actual_x < 11) && (desk[0][actual_x] == 0))
                {
                    if ((actual_x != 2) && (actual_x != 4) && (actual_x != 6) && (actual_x != 8))
                    {
                        moves.push_back({ static_cast<size_t>(actual_x), 0,  m_ns });
                    }
                    m_ns++;
                    actual_x++;
                }
            }
        }
        return moves;
    }



    void run_interal() override
    {
        // by hand and Excel
        // but anyway will try to do some stack based search
        std::vector<std::vector<int64_t>> desk =
        {   //            X         X         X         X
            //  0    1    2    3    4    5    6    7    8    9   10
            {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 ,  0},
            {  -1,  -1,   0,  -1,   0,  -1,   0,  -1,   0,  -1,  -1},
            {  -1,  -1,   0,  -1,   0,  -1,   0,  -1,   0,  -1,  -1},
            {  -1,  -1,   0,  -1,   0,  -1,   0,  -1,   0,  -1,  -1},
            {  -1,  -1,   0,  -1,   0,  -1,   0,  -1,   0,  -1,  -1}
        };


            std::vector<amphipod> amnnipods;
            amphipod S("SHIT", amphipod::amphipod_type::SHIT); amnnipods.push_back(S);
            amphipod A1("A1", amphipod::amphipod_type::A); A1.place(4, 1); amnnipods.push_back(A1); desk[1][4] = 1;
            amphipod A2("A2", amphipod::amphipod_type::A); A2.place(8, 2); amnnipods.push_back(A2); desk[2][8] = 2;
            amphipod A3("A3", amphipod::amphipod_type::A); A3.place(6, 3); amnnipods.push_back(A3); desk[3][6] = 3;
            amphipod A4("A4", amphipod::amphipod_type::A); A4.place(4, 4); amnnipods.push_back(A4); desk[4][4] = 4;

            amphipod B1("B1", amphipod::amphipod_type::B); B1.place(2, 1); amnnipods.push_back(B1); desk[1][2] = 5;
            amphipod B2("B2", amphipod::amphipod_type::B); B2.place(6, 1); amnnipods.push_back(B2); desk[1][6] = 6;
            amphipod B3("B3", amphipod::amphipod_type::B); B3.place(6, 2); amnnipods.push_back(B3); desk[2][6] = 7;
            amphipod B4("B4", amphipod::amphipod_type::B); B4.place(4, 3); amnnipods.push_back(B4); desk[3][4] = 8;

            amphipod C1("C1", amphipod::amphipod_type::C); C1.place(8,1); amnnipods.push_back(C1); desk[1][8] = 9;
            amphipod C2("C2", amphipod::amphipod_type::C); C2.place(4,2); amnnipods.push_back(C2); desk[2][4] = 10;
            amphipod C3("C3", amphipod::amphipod_type::C); C3.place(8,3); amnnipods.push_back(C3); desk[3][8] = 11;
            amphipod C4("C4", amphipod::amphipod_type::C); C4.place(8,4); amnnipods.push_back(C4); desk[4][8] = 12;

            amphipod D1("D1", amphipod::amphipod_type::D); D1.place(2,2); amnnipods.push_back(D1); desk[2][2] = 13;
            amphipod D2("D2", amphipod::amphipod_type::D); D2.place(2,3); amnnipods.push_back(D2); desk[3][2] = 14;
            amphipod D3("D3", amphipod::amphipod_type::D); D3.place(2,4); amnnipods.push_back(D3); desk[4][2] = 15;
            amphipod D4("D4", amphipod::amphipod_type::D); D4.place(6,4); amnnipods.push_back(D4); desk[4][6] = 16;

            for (auto& a : amnnipods)
            {
                std::cout << a.get_name() << std::endl;
                auto moves = find_all_moves(a, desk, amnnipods);
                for (auto m : moves)
                {
                    std::cout << m.t_x << "," << m.t_y << ", " << m.number_of_steps << std::endl;
                }
                std::cout << std::endl;
            }
    }
};



