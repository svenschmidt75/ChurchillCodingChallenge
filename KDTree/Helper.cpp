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
Helper::intersect(std::vector<KDTreeNode const *> const & leafs, Rect const & rect, int32_t const count) {
    auto comp = [](Point const & p1, Point const & p2) {
        return p1.rank > p2.rank;
    };

    using PQ = std::priority_queue<Point, std::vector<Point>, decltype(comp)>;

    concurrency::combinable<std::priority_queue<Point, std::vector<Point>, decltype(comp)>> combinable([comp]() {return std::priority_queue<Point, std::vector<Point>, decltype(comp)>(comp); });
    concurrency::combinable<std::vector<Point>> combinable2([]() {return std::vector<Point>(); });

    std::vector<PQ> pqs;
    pqs.resize(leafs.size(), PQ(comp));

    concurrency::parallel_for(size_t(0), leafs.size(), [&combinable, &combinable2, &leafs, &rect, &pqs, count](size_t leaf_index) {
        auto & pir = combinable2.local();

//        bool res;
//        auto & points_inside_rect = combinable.local(res);
        auto & points_inside_rect = pqs[leaf_index];


        if (points_inside_rect.empty() == false) {
            int a = 1;
            a++;
        }


        auto const leaf = leafs[leaf_index];
        auto const & leaf_points = leaf->points();
        auto max_rank = std::numeric_limits<int32_t>::min();

        if ( leaf_index == 9) {
            int a = 1;
            a++;
        }
        if (leaf->splitting_axis() == 0) {
            // find all points that are in the range [rect.lx, rect.hx]
            // Note: The points MUST be sorted in x!!!
            const auto points_begin = leaf_points.cbegin();
            const auto points_end = leaf_points.cend();
            auto x1 = std::lower_bound(points_begin, points_end, rect.lx, [](Point const & p, float r) {
                return  p.x < r;
            });
            auto x2 = std::upper_bound(points_begin, points_end, rect.hx, [](float r, Point const & p) {
                return  r < p.x;
            });
            if (x1 != points_end) {
                for (; x1 != x2 && x1 != points_end; ++x1) {
                    Point const & p = *x1;
                    if (p.x < rect.lx || p.x > rect.hx)
                        continue;
                    if (p.y >= rect.ly && p.y <= rect.hy) {
                        pir.push_back(p);
                        
                        if (p.rank == 0) {
                            int a = 1;
                            a++;
                        }
                        
                        if (points_inside_rect.size() == count) {
                            int a = 1;
                            a++;
                        }

                        if (points_inside_rect.size() < count || p.rank < max_rank) {
                            points_inside_rect.push(p);
                            max_rank = std::max(max_rank, p.rank);

                            if (max_rank < 1) {
                                int a = 1;
                                a++;
                            }
                        }
                        // if < count, insert
                        // remember max. rank
                        // if number of elements = count, only insert if
                        //  - rank smaller than maximum



                    }
                }
            }
        } else {
            // find all points that are in the range [rect.ly, rect.hy]
            // Note: The points MUST be sorted in y!!!
            const auto points_begin = leaf_points.cbegin();
            const auto points_end = leaf_points.cend();
            auto y1 = std::lower_bound(points_begin, points_end, rect.ly, [](Point const & p, float r) {
                return  p.y < r;
            });
            auto y2 = std::upper_bound(points_begin, points_end, rect.hy, [](float r, Point const & p) {
                return  r < p.y;
            });
            if (y1 != points_end) {
                for (; y1 != y2 && y1 != points_end; ++y1) {
                    Point const & p = *y1;
                    if (p.y < rect.ly || p.y > rect.hy)
                        continue;
                    if (p.x >= rect.lx && p.x <= rect.hx) {
                        pir.push_back(p);


                        if (points_inside_rect.size() == count) {
                            int a = 1;
                            a++;
                        }

                        if (points_inside_rect.size() < count || p.rank < max_rank) {
                            points_inside_rect.push(p);
                            max_rank = std::min(max_rank, p.rank);
                        }
                        // if < count, insert
                        // remember max. rank
                        // if number of elements = count, only insert if
                        //  - rank smaller than maximum
                    }
                }
            }
        }
    });
    std::vector<Point> points_inside_rects;
    points_inside_rects.reserve(count);
    std::priority_queue<Point, std::vector<Point>, decltype(comp)> poi(comp);
    //combinable.combine_each([&points_inside_rects, &poi, count](std::priority_queue<Point, std::vector<Point>, decltype(comp)> points) {
    //    int c = count;
    //    while (c > 0 && points.size() > 0) {
    //        auto const & p = points.top();
    //        points.pop();
    //        poi.push(p);
    //        --c;
    //    }
    //});
    for (auto i = 0; i < pqs.size(); ++i) {
        std::priority_queue<Point, std::vector<Point>, decltype(comp)> & points = pqs[i];
        int c = count;
        while (c > 0 && points.size() > 0) {
            auto const & p = points.top();
            poi.push(p);
            points.pop();
            --c;
        }
    }
    
    int c = count;
    while (c > 0 && poi.size() > 0) {
        auto const & p = poi.top();
        points_inside_rects.push_back(p);
        poi.pop();
        --c;
    }

    
    std::vector<Point> points_inside_rects2;
    combinable2.combine_each([&points_inside_rects2, count](std::vector<Point> points) {
        concurrency::parallel_sort(points.begin(), points.end(), [](Point const & a, Point const & b) {
            return a.rank < b.rank;
        });
        for (auto i = 0, c = 0; i < points.size() && c < count; ++i, ++c) {
            points_inside_rects2.push_back(points[i]);
        }
    });
    concurrency::parallel_sort(points_inside_rects2.begin(), points_inside_rects2.end(), [](Point const & a, Point const & b) {
        return a.rank < b.rank;
    });
    return points_inside_rects;
}

bool
Helper::is_point_in_rect(Point const & p, Rect const & r) {
    return p.x >= r.lx && p.x <= r.hx && p.y >= r.ly && p.y <= r.hy;
}
