#include "pch.h"

#include "KDTree.h"


using namespace KDTree_NS;

struct Point
{
    int8_t id;
    int32_t rank;
    float x;
    float y;
};

struct SearchContext;
typedef SearchContext* (__stdcall* T_create)(const Point* points_begin, const Point* points_end);
