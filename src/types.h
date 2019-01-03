#ifndef TYPES_H
#define TYPES_H

typedef enum
{
    INSIDE,
    OUTSIDE,
    ON
} TPointPolygon;

typedef enum
{
    INTERSECTION,
    UNION,
    DIFFAB,
    DIFFBA
} TBooleanOperation;

typedef enum
{
    PARALLEL,
    COLINEAR,
    INTERSECTING,
    NONINTERSECTING
} T2LinesPosition;

typedef enum
{
    LEFT,
    RIGHT,
    COL
} TPointLinePosition;

#endif // TYPES_H
