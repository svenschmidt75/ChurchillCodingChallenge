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




namespace KDTree_NS {

class KDTREE_EXPORTS_DECL_SYMBOLS KDTree {
public:
    KDTree();
};

} // namespace KDTree_NS
