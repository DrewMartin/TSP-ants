#ifndef COMMON_H
#define COMMON_H

#include <qmath.h>
#include <QPointF>
#include <QPair>

typedef QPair<double, double> BestDistancePheromone;

#define MAX_X 510
#define MAX_Y 510
#define CITY_RADIUS 3
#define ANT_RADIUS 2
#define MAX_GEN_X (MAX_X - 10)
#define MAX_GEN_Y (MAX_Y - 10)
#define FRAME_TIME 33

#define PHEROMONE_MULTIPLIER 100.0
#define PHEROMONE_DEFAULT_DECAY_RATE 50
#define CITY_Z 50
#define LINE_Z 25
#define ANT_Z 75
#define PHEROMONE_Z 10
#define CITY_COLOR "#ff9c00"
#define ANT_COLOR "#ff0000"
#define EDGE_COLOR "#00ff00"
#define BEST_TOUR_COLOR "#00ffff"

#define ANT_SPEED 5.0
#define ANT_PROPORTIONAL_STEPS 25

#define PHEROMONE_WEIGHT 10
#define DISTANCE_WEIGHT 2

#define QSP QSharedPointer
#define LOC_HELPER(loc,rad) loc.x()-rad, loc.y()-rad, rad*2+1, rad*2+1

inline double sqDist(const QPointF &p1, const QPointF &p2) {
    return qPow(p1.x() - p2.x(), 2) + qPow(p1.y() - p2.y(), 2);
}

inline double dist(const QPointF &p1, const QPointF &p2) {
    return qSqrt(sqDist(p1, p2));
}

#endif // COMMON_H
