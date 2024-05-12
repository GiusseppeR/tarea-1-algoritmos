#ifndef TAREALOG_SS_UTILS_H
#define TAREALOG_SS_UTILS_H

#include "../structures.h"

typedef struct cluster{
    Point *array;
    int index_primary_medoide;
} Cluster;

typedef struct closestPoints {
    Point point1;
    Point point2;
    double distance;
}ClosestPoints;

ClosestPoints closest(Point P[], int n);

#endif
