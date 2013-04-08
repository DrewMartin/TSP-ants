#ifndef COMMON_H
#define COMMON_H

#define MAX_X 510
#define MAX_Y 510
#define CITY_RADIUS 3
#define MAX_GEN_X (MAX_X - 10)
#define MAX_GEN_Y (MAX_Y - 10)

#define CITY_Z 50
#define LINE_Z 25
#define CITY_COLOR "#ff9c00"

#define QSP QSharedPointer
#define LOC_HELPER(loc,rad) loc.x()-rad, loc.y()-rad, rad*2+1, rad*2+1

#endif // COMMON_H
