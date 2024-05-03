

#ifndef TAREALOG_UTILS_H
#define TAREALOG_UTILS_H

#include "structures.h"

Point generateRandomPoint();
double squaredDistance(const Point p1, const Point p2);
int minDistance(Point point, Point * samples);
struct ClosestPoints closest(Point P[], int n);
#endif //TAREALOG_UTILS_H
