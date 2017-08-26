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
    auto search_context = new SearchContext;
    search_context->kd_tree = std::make_unique<KDTree>(points_begin, points_end);
    return search_context;
}

int32_t search(SearchContext * /*sc*/, Rect const /*rect*/, int32_t const /*count*/, Point * /*out_points*/) {
    return 0;
}

SearchContext * destroy(SearchContext * sc) {
    sc->kd_tree = nullptr;
    return nullptr;
}




KDTree::KDTree(Point const * /*points_begin*/, Point const  * /*points_end*/) {
}
