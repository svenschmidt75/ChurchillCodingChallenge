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
    for (auto const & leaf : leafs) {
        if (leaf->splitting_axis() == 0) {
            // find all points that are in the range [rect.lx, rect.hx]
            auto x1 = std::lower_bound(leaf->points().cbegin(), leaf->points().cend(), rect.lx, [](Point const & p, float r) {
                return  p.x < r;
            });
            auto x2 = std::upper_bound(leaf->points().cbegin(), leaf->points().cend(), rect.hx, [](float r, Point const & p) {
                return  r < p.x;
            });
            if (x1 != leaf->points().cend() && x2 != leaf->points().cend()) {
                for (; x1 <= x2; ++x1) {
                    Point const & p = *x1;
                    if (p.x < rect.lx || p.x > rect.hx)
                        continue;
                    if (p.y >= rect.ly && p.y <= rect.hy) {
                        //                            std::cout << "Point (" << p.x << "," << p.y << ") is in rect ((" << rect.lx << "," << rect.hx << "),(" << rect.ly << "," << rect.hy << ")..." << std::endl;
                        points_inside_rect.push_back(p);
                    }
                }
            }
        }
        else {
            // find all points that are in the range [rect.ly, rect.hy]
            auto y1 = std::lower_bound(leaf->points().cbegin(), leaf->points().cend(), rect.ly, [](Point const & p, float r) {
                return  p.y < r;
            });
            auto y2 = std::upper_bound(leaf->points().cbegin(), leaf->points().cend(), rect.hy, [](float r, Point const & p) {
                return  r < p.y;
            });
            if (y1 != leaf->points().cend() && y2 != leaf->points().cend()) {
                for (; y1 <= y2; ++y1) {
                    Point const & p = *y1;
                    if (p.y < rect.ly || p.y > rect.hy)
                        continue;
                    if (p.x >= rect.lx && p.x <= rect.hx) {
                        //                            std::cout << "Point (" << p.x << "," << p.y << ") is in rect ((" << rect.lx << "," << rect.hx << "),(" << rect.ly << "," << rect.hy << ")..." << std::endl;
                        points_inside_rect.push_back(p);
                    }
                }
            }
        }
    }
    return points_inside_rect;
}

bool
Helper::is_point_in_rect(Point const & p, Rect const & r) {
    return p.x >= r.lx && p.x <= r.hx && p.y >= r.ly && p.y <= r.hy;
}
