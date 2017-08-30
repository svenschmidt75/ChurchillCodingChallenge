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


std::tuple<float, std::vector<uint64_t>, std::vector<uint64_t>>
Helper::split(std::vector<Point> const & points, std::vector<uint64_t> point_indices, int axis) {
    if (points.empty())
        return std::make_tuple<float, std::vector<uint64_t>, std::vector<uint64_t>>(0, {}, {});

    concurrency::parallel_sort(point_indices.begin(), point_indices.end(), [&points, axis](uint64_t i1, uint64_t i2) {
        auto const & p1 = points[i1];
        auto const & p2 = points[i2];
        return axis == 0 ? p1.x < p2.x : p1.y < p2.y;
    });
    auto const size = point_indices.size();
    float median;
    if (size % 2 == 0) {
        auto const & p1 = points[point_indices[size / 2 - 1]];
        auto const & p2 = points[point_indices[size / 2]];
        median = (axis == 0 ? p1.x + p2.x : p1.y + p2.y) / 2;
    }
    else {
        auto const & p = points[point_indices[size / 2]];
        median = axis == 0 ? p.x : p.y;
    }
    if (size % 2 == 0) {
        auto lower_bound = std::vector<uint64_t>(point_indices.cbegin(), point_indices.cbegin() + size / 2);
        auto upper_bound = std::vector<uint64_t>(point_indices.cbegin() + size / 2, point_indices.cend());
        return std::make_tuple(median, lower_bound, upper_bound);
    }
    // convention: include median value in lower bound
    auto lower_bound = std::vector<uint64_t>(point_indices.cbegin(), point_indices.cbegin() + size / 2 + 1);
    auto upper_bound = std::vector<uint64_t>(point_indices.cbegin() + size / 2 + 1, point_indices.cend());
    return std::make_tuple(median, lower_bound, upper_bound);
}

bool
Helper::is_point_in_rect(Point const & p, Rect const & r) {
    return p.x >= r.lx && p.x <= r.hx && p.y >= r.ly && p.y <= r.hy;
}
