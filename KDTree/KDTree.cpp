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
search(SearchContext * sc, Rect const rect, int32_t const count, Point * out_points) {
    try {
        KDTree const & kdtree = *sc->kd_tree;

        // TODO SS: can this be done in parallel?
        // i.e. one for left subtree, one for right?
        auto leafs = kdtree.intersect_with_rect(rect);
        if (leafs.empty() == false) {
            //if (points_inside_rect.empty() == false) {
            //    int a = 1;
            //    a++;
            //}

            auto points_inside_rect = Helper::intersect(leafs, rect);
            if (points_inside_rect.empty() == false) {
                concurrency::parallel_sort(points_inside_rect.begin(), points_inside_rect.end(), [](Point const & a, Point const & b) {
                    return a.rank < b.rank;
                });
                auto const n_points = std::min(int(leafs.size()), count);
                for (size_t i = 0; i < n_points; ++i) {
                    out_points[i] = points_inside_rect[i];
                }
                return n_points;
            }
        }
        return 0;
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

        if (current_node->num_points() < max_points_per_child)
            continue;

        std::vector<Point> const child_points{std::move(current_node->points())};

        // split points at median
        auto partition = Helper::split(child_points, current_node->splitting_axis());
        current_node->splitting_value_ = std::get<0>(partition);

        uint8_t const child_splitting_axis = (current_node->splitting_axis() + 1) % 2;
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
        if (current_node->is_leaf()) {
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
