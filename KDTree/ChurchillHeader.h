#pragma once

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
