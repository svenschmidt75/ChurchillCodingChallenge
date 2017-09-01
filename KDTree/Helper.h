/*
 * Name  : Helper
 * Path  :
 * Use   : contains helper methods for kd tree creation
 * Author: Sven Schmidt
 * Date  : 08/26/2017
 */
#pragma once

#include "DeclSpec.h"


namespace KDTree_NS {

    
    // forward declarations
    class KDTreeNode;


    class KDTREE_EXPORTS_DECL_SYMBOLS Helper {
    public:
        static std::tuple<float, std::vector<Point>, std::vector<Point>> split(std::vector<Point> points, int axis);
        static std::vector<Point>                                        intersect(std::vector<KDTreeNode const *> const & leafs, Rect const & rect);
        static bool                                                      is_point_in_rect(Point const & p, Rect const & r);
    };

} // namespace KDTree_NS
