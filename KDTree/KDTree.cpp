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

}

int32_t
search(SearchContext * sc, Rect const rect, int32_t const /*count*/, Point * /*out_points*/) {
    try {
        KDTree const & kdtree = *sc->kd_tree;

        // TODO SS: can this be done in parallel?
        // i.e. one for left subtree, one for right?
        auto const leafs = kdtree.intersect_with_rect(rect);

        //std::cout << std::endl << "Rect intersects " << leafs.size() << " leafs..." << std::endl;

        // TODO SS: can this be done in parallel?

        //auto points_inside_rect = extract_points_inside_rect(kdtree.points(), rect, leafs, count);
        //if (points_inside_rect.empty() == false) {
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
/*
            auto const n_points = std::min(int(points_inside_rect.size()), count);

            auto const & p = kdtree.points();
            for (size_t i = 0; i < n_points; ++i) {
                out_points[i] = p[points_inside_rect[i]];
            } 
            return n_points;
        }*/
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

    std::vector<Point> points(points_begin, points_end);
    concurrency::parallel_sort(points.begin(), points.end(), [this](Point const & p1, Point const & p2) {
        return p1.x < p2.x;
    });
    root_ = std::make_unique<KDTreeNode>(points, uint8_t(0));

    // create tree hierarchy
    std::queue<KDTreeNode *> to_process;
    to_process.push(&*root_);
    while (to_process.empty() == false) {
        KDTreeNode * current_node = to_process.front();
        to_process.pop();

        if (current_node->is_leaf())
            continue;

        if (current_node->num_points() < max_points_per_child) {
            std::vector<Point> child_points{current_node->points()};
            uint8_t const child_splitting_axis = (current_node->axis_ + 1) % 2;

            // split points at median
            auto partition = Helper::split(child_points, child_splitting_axis);
            current_node->splitting_value_ = std::get<0>(partition);
            current_node->left_.reset(new KDTreeNode(std::get<1>(partition),child_splitting_axis));
            current_node->right_.reset(new KDTreeNode(std::get<2>(partition), child_splitting_axis));

            //std::cout << std::endl << "Leaf has " << current_node->num_points() << " points" << std::endl;

            // release points at current node
            current_node->points_.clear();

            // split child nodes
            to_process.push(&*current_node->left_);
            to_process.push(&*current_node->right_);
        }
    }
}

uint64_t
KDTree::depth() const {
    return root_->depth();
}

std::vector<Point>
KDTree::intersect_with_rect(Rect const & rect) const {
    std::vector<Point> leafs;

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
