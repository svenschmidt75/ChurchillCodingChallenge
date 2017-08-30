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

    concurrency::parallel_sort(points.begin(), points.end(), [axis](Point const & a, Point const & b) {
        return axis == 0 ? a.x < b.x : a.y < b.y;
    });
    auto const size = points.size();
    float median;
    if (size % 2 == 0) {
        if (axis == 0)
            median = (points[size / 2 - 1].x + points[size / 2].x) / 2;
        else
            median = (points[size / 2 - 1].y + points[size / 2].y) / 2;
    } else {
        if (axis == 0)
            median = points[size / 2].x;
        else
            median = points[size / 2].y;
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

bool
Helper::is_point_in_rect(Point const & p, Rect const & r) {
    return p.x >= r.lx && p.x <= r.hx && (p.y >= r.ly && p.y <= r.hy);
}
