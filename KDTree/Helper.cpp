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


using namespace KDTree_NS;


std::tuple<std::vector<Point>, std::vector<Point>>
Helper::split(std::vector<Point> points, int /*axis*/) {
    // TODO SS: use parallel policy
    //std::sort(points.begin(), points.end(), [axis](Point const & a, Point const & b) {
    //    return axis == 0 ? a.x < b.x : a.y < b.y;
    //});
    auto const size = points.size();

    // get the median value
    //float median;
    //if (size % 2 == 0) {
    //    if (axis == 0)
    //        median = (points[size / 2 - 1].x + points[size / 2].x) / 2;
    //    else
    //        median = (points[size / 2 - 1].y + points[size / 2].y) / 2;
    //} else {
    //    if (axis == 0)
    //        median = points[size / 2 + 1].x / 2;
    //    else
    //        median = points[size / 2 + 1].y / 2;
    //}

    //if (size % 2 == 0) {
    //    //auto lower_bound = std::vector<Point>(points.cbegin(), points.cbegin() + size / 2);
    //    //auto upper_bound = std::vector<Point>(points.cbegin() + size / 2, points.cend());
    //    std::vector<Point> lower_bound = std::vector<Point>();
    //    std::vector<Point> upper_bound = std::vector<Point>();
    //    return std::make_tuple(lower_bound, upper_bound);
    //}
    //// convention: include median value in lower bound
    //auto lower_bound = std::vector<Point>(&points[0], &points[size / 2 + 1]);
    //auto upper_bound = std::vector<Point>(&points[size / 2 + 1], &points[size]);
    //return std::make_tuple(lower_bound, upper_bound);
    return std::make_tuple(std::vector<Point>(), std::vector<Point>());
}
