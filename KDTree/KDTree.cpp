/*
 * Name  : KDTree.h
 * Path  :
 * Use   : defines a kd tree
 * Author: Sven Schmidt
 * Date  : 08/26/2017
 */
#include "pch.h"

#include "ChurchillHeader.h"
#include "KDTree.h"
#include "KDTreeNode.h"
#include "Helper.h"


using namespace KDTree_NS;


struct SearchContext {
    std::unique_ptr<KDTree> kd_tree;
};


SearchContext *
create(Point const * points_begin, Point const  * points_end) {
    try {
        auto search_context = new SearchContext;
        search_context->kd_tree = std::make_unique<KDTree>(KDTree::MAX_POINTS_PER_LEAF, points_begin, points_end);
        //auto const depth = search_context->kd_tree->Depth();
        //std::cout << std::endl << "Created tree of depth " << depth << std::endl;
        return search_context;
    }
    catch(...) {}
    return nullptr;
}

namespace {
    
    std::vector<uint64_t>
    extract_points_inside_rect(std::vector<Point> const & points, Rect const & rect, std::vector<KDTreeNode const *> const & leafs, int32_t const count) {
        std::vector<uint64_t> points_inside_rect;
        points_inside_rect.reserve(leafs.size() * KDTree::MAX_POINTS_PER_LEAF);

        // TODO SS: do this in parallel, i.e. for each leaf with points, we extact the points
        // inside the rect
        // At the end, we extract the 1st count from each one and merge them and resort them.
        // We then return the 1st count.


        for (auto const & leaf : leafs) {
            auto const & point_indices = leaf->points();
            //std::back_insert_iterator<std::vector<Point>> const back_it(points_inside_rect);
            //std::copy_if(points.cbegin(), points.cend(), back_it, [&rect](Point p) {
            //    bool point_inside_rect = Helper::is_point_in_rect(p, rect);
            //    return point_inside_rect;
            //});

            int cnt = 0;
            for (auto const & point : point_indices) {
                if (cnt == count)
                    break;
                bool const point_inside_rect = Helper::is_point_in_rect(points[point], rect);
                if (point_inside_rect) {
                    points_inside_rect.push_back(point);
                    ++cnt;
                }
            }
        }


        return points_inside_rect;
    }


    std::vector<std::vector<uint64_t>>
    extract_points_inside_rect3(std::vector<Point> const & points, Rect const & rect, std::vector<KDTreeNode const *> const & leafs, int32_t const count) {
//        std::vector<std::vector<Point>> pps;
//        pps.reserve(leafs.size());
//
//        // TODO SS: do this in parallel, i.e. for each leaf with points, we extact the points
//        // inside the rect
//        // At the end, we extract the 1st count from each one and merge them and resort them.
//        // We then return the 1st count.
//
//
//        for (auto const & leaf : leafs) {
//            std::vector<Point> points_inside_rect;
//            points_inside_rect.reserve(count);
//            auto const & points = leaf->points();
//            int cnt = 0;
//            for (auto const & point : points) {
//                if (cnt == count)
//                    break;
//                bool const point_inside_rect = Helper::is_point_in_rect(point, rect);
//                if (point_inside_rect) {
//                    points_inside_rect.push_back(point);
//                    ++cnt;
//                }
//            }
//            pps.push_back(points_inside_rect);
//        }
////       return pps;
//    
        std::vector<std::vector<uint64_t>> pps2;
        pps2.resize(leafs.size());

        concurrency::parallel_for(size_t(0), leafs.size(), size_t(1), [&pps2, &rect, &leafs, &points, count](size_t index) {
            auto & vector = pps2[index];
            vector.reserve(count);
            auto const & point_indices = leafs[index]->points();
            int cnt = 0;
            for (auto const & point : point_indices) {
                if (cnt == count)
                    break;
                bool const point_inside_rect = Helper::is_point_in_rect(points[point], rect);
                if (point_inside_rect) {
                    vector.emplace_back(point);
                    ++cnt;
                }
            }
        });

        //for (auto const & ps : pps2)
        //    pps2.push_back(ps);

        return pps2;

        //concurrency::parallel_for(size_type{ 0 }, size, chunk_size, [&part_sums, &v1, &v2, vector_size, numberOfProcessors](size_type index) {
        //    size_type start_row, end_size;
        //    std::tie(start_row, end_size) = common_NS::getChunkStartEndIndex(vector_size, size_type{ numberOfProcessors }, index);
        //    double part_result = 0;
        //    for (size_type i = start_row; i < end_size; ++i) {
        //        double tmp = v1(i) * v2(i);
        //        part_result += tmp;
        //    }
        //    part_sums.local() += part_result;
        //}, concurrency::static_partitioner());






    }


    struct N {
        explicit N(std::vector<uint64_t> const & points)
            : points{ points }, pos{ 0 } {}

    public:
        std::vector<uint64_t> const & points;
        int                           pos;
    };

    std::vector<uint64_t>
    extract_points_inside_rect2(std::vector<Point> const & points, Rect const & rect, std::vector<KDTreeNode const *> const & leafs, int32_t const count) {
        std::vector<uint64_t> point_indices;

        if (leafs.empty())
            return point_indices;

        point_indices.reserve(count);

        auto pps = extract_points_inside_rect3(points, rect, leafs, count);
        std::vector<N> Ns;
        for (auto const & ps : pps) {
            if (ps.empty() == false) {
                Ns.emplace_back(N{ ps });
            }
        }

        int c = 0;
        while (c < count) {
            int index_minimum = -1;
            int min_rank = std::numeric_limits<int32_t>::max();
            // find minimum rank
            for (int i = 0; i < Ns.size(); ++i) {
                auto & n = Ns[i];
                if (n.pos < n.points.size()) {
                    auto const & point = points[n.points[n.pos]];
                    auto rank = point.rank;
                    if (rank < min_rank) {
                        index_minimum = i;
                        min_rank = rank;
                    }
                }
            }
            if (index_minimum >= 0) {
                auto & n = Ns[index_minimum];
                point_indices.push_back(n.points[n.pos]);
                ++n.pos;
            }
            ++c;
        }

        return point_indices;
    }

}

int32_t
search(SearchContext * sc, Rect const rect, int32_t const count, Point * out_points) {
    try {
        KDTree const & kdtree = *sc->kd_tree;

        // TODO SS: can this be done in parallel?
        // i.e. one for left subtree, one for right?
        auto const leafs = kdtree.intersect_with_rect(rect);

        // TODO SS: can this be done in parallel?

        //auto points_inside_rect = extract_points_inside_rect(rect, leafs, count);
        auto points_inside_rect = extract_points_inside_rect2(kdtree.points(), rect, leafs, count);
        if (points_inside_rect.empty() == false) {
            //std::cout << std::endl << "Number of points in rect to sort: " << points_inside_rect.size() << std::endl;

            //if (points_inside_rect.empty() == false) {
            //    int a = 1;
            //    a++;
            //}

            //std::sort(points_inside_rect.begin(), points_inside_rect.end(), [](Point const & a, Point const & b) {
            //    return a.rank < b.rank;
            //});

            //concurrency::parallel_sort(points_inside_rect.begin(), points_inside_rect.end(), [](Point const & a, Point const & b) {
            //    return a.rank < b.rank;
            //});

            auto const n_points = std::min(int(points_inside_rect.size()), count);

            for (size_t i = 0; i < n_points; ++i) {
                auto const & p = kdtree.points();
                out_points[i] = p[points_inside_rect[i]];
            }
            return n_points;
        }
        return 0;





//        std::vector<Point> points_inside_rect;




        //concurrency::parallel_for(size_type{ 0 }, size, chunk_size, [&part_sums, &v1, &v2, vector_size, numberOfProcessors](size_type index) {
        //    size_type start_row, end_size;
        //    std::tie(start_row, end_size) = common_NS::getChunkStartEndIndex(vector_size, size_type{ numberOfProcessors }, index);
        //    double part_result = 0;
        //    for (size_type i = start_row; i < end_size; ++i) {
        //        double tmp = v1(i) * v2(i);
        //        part_result += tmp;
        //    }
        //    part_sums.local() += part_result;
        //}, concurrency::static_partitioner());



        //concurrency::parallel_invoke(
        //    [this] {finalizeColumnIndices(); },
        //    [this] {finalizeRowIndices(); },
        //    [this] {finalizeElements(); }
        //);



    }
    catch(...) {}
    return -1;
}

SearchContext *
destroy(SearchContext * sc) {
    try {
        sc->kd_tree.reset();
        return nullptr;
    }
    catch (...) {}
    return sc;
}


namespace {
    
    std::vector<uint64_t>
    generate_sequence(uint64_t size) {
        std::vector<uint64_t> sequence;
        sequence.resize(size);
        int n = {0};
        std::generate(sequence.begin(), sequence.end(), [&n] { return n++; });
        return sequence;
    }
}


KDTree::KDTree(uint64_t max_points_per_child,  Point const * points_begin, Point const * points_end)
    :
    points_{std::vector<Point>(points_begin, points_end)},
    root_{std::make_unique<KDTreeNode>(generate_sequence(points_.size()), uint8_t(0))},
    max_points_per_child_{max_points_per_child} {
    


    // TODO SS: only split both x and y once, as Point vector is static
    // TODO SS: as Point vector is static, arrange hierarchy in array, rather than nodes etc. (better cache utilization?)




    // subdivide tree
    std::queue<KDTreeNode *> to_process;
    to_process.push(&*root_);
    while (to_process.empty() == false) {
        KDTreeNode * current_node = to_process.front();
        to_process.pop();

        if (current_node->num_points() <= max_points_per_child) {
            // leaf node, sort points by rank
            concurrency::parallel_sort(current_node->points_.begin(), current_node->points_.end(), [this](uint64_t i1, uint64_t i2) {
                auto const & p1 = points_[i1];
                auto const & p2 = points_[i2];
                return p1.rank < p2.rank;
            });
            continue;
        }

        // split points at median
        auto partition = Helper::split(points_, current_node->points_, current_node->axis_);
        current_node->splitting_value_ = std::get<0>(partition);
        current_node->left_.reset(new KDTreeNode(std::get<1>(partition), (current_node->axis_ + 1) % 2));
        current_node->right_.reset(new KDTreeNode(std::get<2>(partition), (current_node->axis_ + 1) % 2));

        // release points at current node
        current_node->points_.clear();

        // split child nodes
        to_process.push(&*current_node->left_);
        to_process.push(&*current_node->right_);
    }
}

uint64_t
KDTree::depth() const {
    return root_->depth();
}

std::vector<KDTreeNode const *>
KDTree::intersect_with_rect(Rect const & rect) const {
    std::vector<KDTreeNode const *> leafs;

    std::queue<KDTreeNode const *> to_process;
    to_process.push(&*root_);
    while (to_process.empty() == false) {
        KDTreeNode const * current_node = to_process.front();
        to_process.pop();
        if (current_node == nullptr)
            continue;
        if (current_node->points_.empty() == false) {
            // leaf node
            leafs.push_back(current_node);
        } else {
            if (current_node->rect_intersects_left_subtree(rect)) {
                // not a leaf node
                to_process.push(&*current_node->left_);
            }
            if (current_node->rect_intersects_right_subtree(rect)) {
                // not a leaf node
                to_process.push(&*current_node->right_);
            }
        }
    }
    return leafs;
}

std::vector<Point> const &
KDTree::points() const {
    return points_;
}
