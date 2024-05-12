#include "utils.h"
#include "random/custom_random.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <stdlib.h>

Point generateRandomPoint(){
    Point *output = malloc(sizeof(Point) );
    output->x = xoshiro256plus_next();
    output->y = xoshiro256plus_next();
    return *output;
}

double squaredDistance(const Point p1, const Point p2){
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;
    return dx * dx + dy * dy;
}

int equals(Point p1, Point p2) {
    return (p1.x == p2.x) & (p1.y == p2.y);
}
