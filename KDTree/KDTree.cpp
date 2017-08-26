#include "pch.h"

#include "KDTree.h"
#include <iostream>


using namespace KDTree_NS;


/* The following structs are packed with no padding. */
#pragma pack(push, 1)

/* Defines a point in 2D space with some additional attributes like id and rank. */
struct Point {
    int8_t  id;
    int32_t rank;
    float   x;
    float   y;
};

/* Defines a rectangle, where a point (x,y) is inside, if x is in [lx, hx] and y is in [ly, hy]. */
struct Rect {
    float lx;
    float ly;
    float hx;
    float hy;
};

#pragma pack(pop)


struct SearchContext {
    std::unique_ptr<KDTree> kd_tree;
};

SearchContext * create(Point const * /*points_begin*/, Point const  * /*points_end*/) {
    auto search_context = new SearchContext;
    search_context->kd_tree = std::make_unique<KDTree>();
    return search_context;
}

int32_t search(SearchContext * /*sc*/, Rect const /*rect*/, int32_t const /*count*/, Point * /*out_points*/) {
    return 0;
}

SearchContext * destroy(SearchContext * sc) {
    sc->kd_tree = nullptr;
    return nullptr;
}




KDTree::KDTree() {
}
