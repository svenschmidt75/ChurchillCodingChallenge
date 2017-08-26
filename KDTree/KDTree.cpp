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


using namespace KDTree_NS;


struct SearchContext {
    std::unique_ptr<KDTree> kd_tree;
};

SearchContext * create(Point const * points_begin, Point const  * points_end) {
    try {
        auto search_context = new SearchContext;
        search_context->kd_tree = std::make_unique<KDTree>(points_begin, points_end);
        return search_context;
    }
    catch(...) {}
    return nullptr;
}

int32_t search(SearchContext * /*sc*/, Rect const /*rect*/, int32_t const /*count*/, Point * /*out_points*/) {
    try {
        return 0;
    }
    catch(...) {}
    return -1;
}

SearchContext * destroy(SearchContext * sc) {
    try {
        sc->kd_tree = nullptr;
        return nullptr;
    }
    catch (...) {}
    return sc;
}




KDTree::KDTree(Point const * points_begin, Point const  * points_end)
    :
    root_{std::make_unique<KDTreeNode>(std::vector<Point>(points_begin, points_end), 0)} {}
