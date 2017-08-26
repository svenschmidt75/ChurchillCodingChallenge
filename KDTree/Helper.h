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

    class KDTREE_EXPORTS_DECL_SYMBOLS Helper {
    public:
        static std::tuple<std::vector<Point>, std::vector<Point>> split(std::vector<Point> points, int axis);
    };

} // namespace KDTree_NS
