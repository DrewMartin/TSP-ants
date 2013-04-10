#ifndef COMMON_H
#define COMMON_H

#include "qmath.h"
#include <QPoint>

#define MAX_X 510
#define MAX_Y 510
#define CITY_RADIUS 3
#define ANT_RADIUS 2
#define MAX_GEN_X (MAX_X - 10)
#define MAX_GEN_Y (MAX_Y - 10)

#define CITY_Z 50
#define LINE_Z 25
#define ANT_Z 75
#define CITY_COLOR "#ff9c00"
#define ANT_COLOR "#ff0000"

#define QSP QSharedPointer
#define LOC_HELPER(loc,rad) loc.x()-rad, loc.y()-rad, rad*2+1, rad*2+1

inline int sqDist(const QPoint &p1, const QPoint &p2) {
    return qPow(p1.x() - p2.x(), 2) + qPow(p1.y() - p2.y(), 2);
}

inline int dist(const QPoint &p1, const QPoint &p2) {
    return qSqrt(sqDist(p1, p2));
}

#endif // COMMON_H
