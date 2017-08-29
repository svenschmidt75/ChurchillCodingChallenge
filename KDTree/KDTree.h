/*
 * Name  : KDTree.h
 * Path  :
 * Use   : declares a kd tree
 * Author: Sven Schmidt
 * Date  : 08/26/2017
 */
#pragma once

#include "DeclSpec.h"


#ifdef __cplusplus  
extern "C" {  // only need to export C interface if  
              // used by C++ source code  
#endif  

    // forward declarations
    struct SearchContext;
    struct Point;
    struct Rect;

    __declspec(dllexport) SearchContext * (__stdcall create)(Point const * points_begin, Point const * points_end);
    __declspec(dllexport) int32_t(__stdcall search)(SearchContext * sc, Rect const rect, int32_t const count, Point * out_points);
    __declspec(dllexport) SearchContext * (__stdcall destroy)(SearchContext * sc);

#ifdef __cplusplus  
}
#endif  


#pragma warning(disable:4251)


namespace KDTree_NS {


    // forward declarations
    class KDTreeNode;


    class KDTREE_EXPORTS_DECL_SYMBOLS KDTree {
    public:
        static uint64_t const MAX_POINTS_PER_LEAF = 2000;

    public:
        explicit KDTree(uint64_t max_points_per_child, Point const * points_begin, Point const * points_end);

        uint64_t                        depth() const;
        std::vector<KDTreeNode const *> intersect_with_rect(Rect const & rect) const;
        std::vector<Point> const &      points() const;

    private:
        std::vector<Point>          points_;
        std::unique_ptr<KDTreeNode> root_;
        uint64_t const              max_points_per_child_;
    };

} // namespace KDTree_NS

#pragma warning(default:4251)
