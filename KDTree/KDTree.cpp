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
        search_context->kd_tree = std::make_unique<KDTree>(uint8_t(100u), points_begin, points_end);
        //auto const depth = search_context->kd_tree->Depth();
        //std::cout << std::endl << "Created tree of depth " << depth << std::endl;
        return search_context;
    }
    catch(...) {}
    return nullptr;
}

int32_t
search(SearchContext * /*sc*/, Rect const /*rect*/, int32_t const /*count*/, Point * /*out_points*/) {
    try {
        return 0;
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




KDTree::KDTree(uint8_t max_points_per_child,  Point const * points_begin, Point const  * points_end)
    :
    root_{std::make_unique<KDTreeNode>(std::vector<Point>(points_begin, points_end), uint8_t(0))},
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
            continue;
        }

        // split points at median
        auto partition = Helper::split(current_node->points_, current_node->axis_);
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
KDTree::intersect_with_rect(Rect const rect) const {
    std::vector<KDTreeNode const *> leafs;

    std::queue<KDTreeNode const *> to_process;
    to_process.push(&*root_);
    while (to_process.empty() == false) {
        KDTreeNode const * current_node = to_process.front();
        to_process.pop();
        assert(current_node != nullptr);
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
