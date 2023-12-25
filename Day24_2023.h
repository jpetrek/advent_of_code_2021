#pragma once
#include "helper.h"
#include <random>
#include <cmath>
#define _SILENCE_CXX23_DENORM_DEPRECATION_WARNING
#define _SILENCE_ALL_CXX23_DEPRECATION_WARNINGS
#include <Eigen\Core>
#include <Eigen\Dense>

struct point
{
    double x, y, z;
};

Eigen::Matrix3d crossMatrix(Eigen::Vector3d v)
{
    Eigen::Matrix3d result;
    result << 0, -v[2], v[1], v[2], 0, -v[0], -v[1], v[0], 0; return result;
}

template<>
class day<24, 2023> : public day_base<24, 2023>
{

    struct intersection : public point
    {
        bool parallel;
        bool collinear;
        enum extends_type { none = 0, AB = 1, BA = 2, CD = 4, DC = 8 } extends;

        intersection(point A, point B, point C, point D, double epsilon = 1e-30) :
            parallel(false),
            collinear(false),
            extends(none)
        {
            double d = denominator(A, B, C, D);

            if (std::abs(d) < epsilon)
            {
                parallel = true;
                collinear = abs(numerator(A, C, C, D)) < epsilon;
                return;
            }

            double r = numerator(A, C, C, D) / d;
            double s = numerator(A, C, A, B) / d;

            x = A.x + r * (B.x - A.x);
            y = A.y + r * (B.y - A.y);

            extends = (extends_type)((r > 1) + 2 * (r < 0) + 4 * (s > 1) + 8 * (s < 0));
        }

    private:
        inline double numerator(point A, point C, point E, point F) { return (A.y - C.y) * (F.x - E.x) - (A.x - C.x) * (F.y - E.y); }
        inline double denominator(point A, point B, point C, point D) { return (B.x - A.x) * (D.y - C.y) - (B.y - A.y) * (D.x - C.x); }
    };


    std::pair<bool, point> intersect_2d_with_proper_direction( const point p1, const point p2, const point p3, const point p4, double epsilon = 1e-30)
    {
        auto p = intersection(p1,p2,p3,p4, epsilon);
        if (p.parallel) return { false, {0,0,0} };
        if (p.extends != 5) return { false, {0,0,0} };
        return {true, p};
    }
  
    void run_internal() override
    {
        struct Hailstone
        {
            Eigen::Vector3d p;
            Eigen::Vector3d v;
        };

        std::vector<Hailstone> hs;

        while (!input_reader().is_end_of_file())
        {
            auto l = helper::split(input_reader().get_line(), '@');
            auto ps = helper::split_and_convert<double>(l[0], ',', [](const auto& s) {return std::stold(s); });
            auto ds = helper::split_and_convert<double>(l[1], ',', [](const auto& s) {return std::stold(s); });
            Hailstone h;
            h.p[0] = ps[0]; h.p[1] = ps[1];  h.p[2] = ps[2];
            h.v[0] = ds[0]; h.v[1] = ds[1];  h.v[2] = ds[2];
            hs.push_back(h);
        }

        const double lb = 200000000000000;
        const double rb = 400000000000000;

        size_t s1 = 0;
        for (size_t i = 0; i < hs.size(); i++)
            for (size_t j = 0; j < hs.size(); j++)
            {
                if (i != j)
                {
                    auto p = intersect_2d_with_proper_direction({hs[i].p[0], hs[i].p[1], hs[i].p[2]}, {hs[i].p[0] + hs[i].v[0], hs[i].p[1] + hs[i].v[1], hs[i].p[2] + hs[i].v[2] },
                                          {hs[j].p[0], hs[j].p[1], hs[j].p[2]}, {hs[j].p[0] + hs[j].v[0], hs[j].p[1] + hs[j].v[1], hs[j].p[2] + hs[j].v[2] });
                    if (p.first)
                    {
                        if ((p.second.x >= lb) && (p.second.y >= lb) && (p.second.x <= rb) && (p.second.y <= rb))
                        {
                            s1++;
                        }
                    }
                }
            }
       set_star1_result(s1 / 2);

       Eigen::MatrixXd M(6, 6);
       Eigen::VectorXd rhs(6);
       rhs.segment<3>(0) = -hs[0].p.cross(hs[0].v) + hs[1].p.cross(hs[1].v);
       rhs.segment<3>(3) = -hs[0].p.cross(hs[0].v) + hs[2].p.cross(hs[2].v);
       M.block<3, 3>(0, 0) = crossMatrix(hs[0].v) - crossMatrix(hs[1].v);
       M.block<3, 3>(3, 0) = crossMatrix(hs[0].v) - crossMatrix(hs[2].v);
       M.block<3, 3>(0, 3) = -crossMatrix(hs[0].p) + crossMatrix(hs[1].p);
       M.block<3, 3>(3, 3) = -crossMatrix(hs[0].p) + crossMatrix(hs[2].p);
       Eigen::VectorXd result = M.inverse()* rhs;
       int64_t s2_1 = 0;
       int64_t s2_2 = 0;
       for (int i = 0; i < 3; i++)
       {
           s2_1 += int64_t(std::ceil(result[i]));
           s2_2 += int64_t(std::floor(result[i]));
       }

       std::cout << s2_1 << std::endl;
       std::cout << s2_2 << std::endl;

       set_star2_result(s2_1);
    }

};
