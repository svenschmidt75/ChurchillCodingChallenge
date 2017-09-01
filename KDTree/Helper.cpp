/*
* Name  : Helper
* Path  :
* Use   : contains helper methods for kd tree creation
* Author: Sven Schmidt
* Date  : 08/26/2017
*/
#include "pch.h"

#include "ChurchillHeader.h"
#include "Helper.h"
#include "KDTreeNode.h"


using namespace KDTree_NS;


std::tuple<float, std::vector<Point>, std::vector<Point>>
Helper::split(std::vector<Point> points, int axis) {
    if (points.empty())
        return std::make_tuple<float, std::vector<Point>, std::vector<Point>>(0, {}, {});
    if (axis == 0) {
        concurrency::parallel_sort(points.begin(), points.end(), [](Point const & p1, Point const & p2) {
            return p1.x < p2.x;
        });
    }
    else {
        concurrency::parallel_sort(points.begin(), points.end(), [](Point const & p1, Point const & p2) {
            return p1.y < p2.y;
        });
    }
    auto const size = points.size();
    float median;
    if (size % 2 == 0) {
        auto const & p1 = points[size / 2 - 1];
        auto const & p2 = points[size / 2];
        median = (axis == 0 ? p1.x + p2.x : p1.y + p2.y) / 2;
    }
    else {
        auto const & p = points[size / 2];
        median = axis == 0 ? p.x : p.y;
    }
    if (size % 2 == 0) {
        auto lower_bound = std::vector<Point>(points.cbegin(), points.cbegin() + size / 2);
        auto upper_bound = std::vector<Point>(points.cbegin() + size / 2, points.cend());
        return std::make_tuple(median, lower_bound, upper_bound);
    }
    // convention: include median value in lower bound
    auto lower_bound = std::vector<Point>(points.cbegin(), points.cbegin() + size / 2 + 1);
    auto upper_bound = std::vector<Point>(points.cbegin() + size / 2 + 1, points.cend());
    return std::make_tuple(median, lower_bound, upper_bound);
}

std::vector<Point>
Helper::intersect(std::vector<KDTreeNode const *> const & leafs, Rect const & rect) {
    std::vector<Point> points_inside_rect;
    for (auto leaf : leafs) {
        std::vector<Point> leaf_points = leaf->points();
        concurrency::parallel_sort(leaf_points.begin(), leaf_points.end(), [](Point const & a, Point const & b) {
            return a.x < b.x;
        });
        const auto points_begin = leaf_points.cbegin();
        const auto points_end = leaf_points.cend();
        if (leaf->splitting_axis() == 0) {
            // find all points that are in the range [rect.lx, rect.hx]
            // Note: The points MUST be sorted in x!!!
            auto x1 = std::lower_bound(points_begin, points_end, rect.lx, [](Point const & p, float r) {
                return  p.x < r;
            });
            auto x2 = std::upper_bound(points_begin, points_end, rect.hx, [](float r, Point const & p) {
                return  r < p.x;
            });



            std::vector<Point> ps = leaf->points();
            std::vector<Point> ps1;
            concurrency::parallel_sort(ps.begin(), ps.end(), [](Point const & a, Point const & b) {
                return a.x < b.x;
            });
            for (auto i = 0; i < ps.size(); ++i) {
                Point const p = ps[i];
                if (Helper::is_point_in_rect(p, rect)) {
                    ps1.push_back(p);
                }
            }

            std::vector<Point> ps2;


            if (x1 != points_end) {
                for (; x1 != x2 && x1 != points_end; ++x1) {
                    Point const & p = *x1;
                    if (p.x < rect.lx || p.x > rect.hx)
                        continue;
                    if (p.y >= rect.ly && p.y <= rect.hy) {
                        //                            std::cout << "Point (" << p.x << "," << p.y << ") is in rect ((" << rect.lx << "," << rect.hx << "),(" << rect.ly << "," << rect.hy << ")..." << std::endl;
                        points_inside_rect.push_back(p);
                        ps2.push_back(p);
                    }
                }
            }

            if (ps1.size() != ps2.size()) {
                int a = 1;
                a++;
            }

        }
        else {
            // find all points that are in the range [rect.ly, rect.hy]
            // Note: The points MUST be sorted in y!!!
            auto y1 = std::lower_bound(points_begin, points_end, rect.ly, [](Point const & p, float r) {
                return  p.y < r;
            });
            auto y2 = std::upper_bound(points_begin, points_end, rect.hy, [](float r, Point const & p) {
                return  r < p.y;
            });



            std::vector<Point> ps1;
            for (auto i = 0; i < leaf->points().size(); ++i) {
                Point const p = leaf->points()[i];
                if (Helper::is_point_in_rect(p, rect)) {
                    ps1.push_back(p);
                }
            }


            concurrency::parallel_sort(ps1.begin(), ps1.end(), [](Point const & a, Point const & b) {
                return a.y < b.y;
            });

            std::vector<Point> ps2;

            if (y1 != points_end) {
                for (; y1 != y2 && y1 != points_end; ++y1) {
                    Point const & p = *y1;
                    if (p.y < rect.ly || p.y > rect.hy)
                        continue;
                    if (p.x >= rect.lx && p.x <= rect.hx) {
                        //                            std::cout << "Point (" << p.x << "," << p.y << ") is in rect ((" << rect.lx << "," << rect.hx << "),(" << rect.ly << "," << rect.hy << ")..." << std::endl;
                        points_inside_rect.push_back(p);
                        ps2.push_back(p);
                    }
                }
            }


            if (ps1.size() != ps2.size()) {
                int a = 1;
                a++;
            }
        }
    }
    return points_inside_rect;
}

bool
Helper::is_point_in_rect(Point const & p, Rect const & r) {
    return p.x >= r.lx && p.x <= r.hx && p.y >= r.ly && p.y <= r.hy;
}
