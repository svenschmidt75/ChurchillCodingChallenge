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
#include <map>


using namespace KDTree_NS;


struct SearchContext {
    std::unique_ptr<KDTree> kd_tree;
};


SearchContext *
create(Point const * points_begin, Point const  * points_end) {
    try {
        auto search_context = new SearchContext;
        search_context->kd_tree = std::make_unique<KDTree>(KDTree::MAX_POINTS_PER_LEAF, points_begin, points_end);
        //auto const depth = search_context->kd_tree->depth();
        //std::cout << std::endl << "Created tree of depth " << depth << std::endl;
        return search_context;
    }
    catch (...) {}
    return nullptr;
}

namespace {

    std::vector<uint64_t>
    extract_points_inside_rect(std::vector<Point> const & points, Rect const & rect, std::vector<KDTreeNode const *> const & leafs, int32_t const count) {
        std::vector<uint64_t> ps(points.size(), points.size());
        for (auto const & leaf : leafs) {
            auto const & leaf_points = leaf->points();
            int cnt = 0;
            for (uint64_t pi : leaf_points) {
                auto const & p = points[pi];
                if (Helper::is_point_in_rect(p, rect)) {
                    if (ps[p.rank] < points.size())
                        throw std::runtime_error("should not be != 0");
                    ps[p.rank] = pi;
                    if (++cnt == count)
                        break;
                }
            }
        }
        std::vector<uint64_t> finals;
        finals.reserve(count);
        int cnt = 0;
        for (auto i = 0; i < ps.size(); ++i) {
            if (ps[i] < points.size()) {
                finals.push_back(ps[i]);
                if (++cnt == count)
                    break;
            }
        }
        return finals;
    }

}

int32_t
search(SearchContext * sc, Rect const rect, int32_t const count, Point * out_points) {
    try {
        KDTree const & kdtree = *sc->kd_tree;

        // TODO SS: can this be done in parallel?
        // i.e. one for left subtree, one for right?
        auto const leafs = kdtree.intersect_with_rect(rect);

        //std::cout << std::endl << "Rect intersects " << leafs.size() << " leafs..." << std::endl;

        // TODO SS: can this be done in parallel?

        auto points_inside_rect = extract_points_inside_rect(kdtree.points(), rect, leafs, count);
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

            auto const & p = kdtree.points();
            for (size_t i = 0; i < n_points; ++i) {
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
    catch (...) {}
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
        int n = { 0 };
        std::generate(sequence.begin(), sequence.end(), [&n] { return n++; });
        return sequence;
    }
}

KDTree::KDTree(uint64_t max_points_per_child, Point const * points_begin, Point const * points_end)
    :
    max_points_per_child_{max_points_per_child} {

    // TODO SS: as Point vector is static, arrange hierarchy in array, rather than nodes etc. (better cache utilization?)

    points_x_.assign(points_begin, points_end);
    concurrency::parallel_sort(points_x_.begin(), points_x_.end(), [this](Point const & p1, Point const & p2) {
        return p1.x < p2.x;
    });

    points_y_.assign(points_begin, points_end);
    concurrency::parallel_sort(points_y_.begin(), points_y_.end(), [this](Point const & p1, Point const & p2) {
        return p1.y < p2.y;
    });

    root_ = std::make_unique<KDTreeNode>(generate_sequence(points_x_.size()), uint8_t(0), 0, points_x_.size() - 1, 0, points_y_.size() - 1);


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

            //std::cout << std::endl << "Leaf has " << current_node->num_points() << " points" << std::endl;

            continue;
        }

        // split points at median
//        auto partition = Helper::split(points_, current_node->points_, current_node->axis_);
        if (current_node->axis_ == 0)
        {
            auto arity = (current_node->xmax() - current_node->xmin() + 1) % 2;
            if (arity == 0) {
                auto median_index = (current_node->xmax() - current_node->xmin() + 1) / 2;
                auto splitting_value = (points_x_[median_index - 1].x + points_x_[median_index].x) / 2;


                int n_left = median_index - 1 - current_node->xmin();
                int n_right = median_index - current_node->xmax();


                current_node->splitting_value_ = splitting_value;
                current_node->left_.reset(new KDTreeNode(points_x_   , (current_node->axis_ + 1) % 2));
                current_node->right_.reset(new KDTreeNode(std::get<2>(partition), (current_node->axis_ + 1) % 2));
            }

        }
        

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
        }
        else {
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
