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

#endif // TYPES_H
