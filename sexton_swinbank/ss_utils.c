#include <math.h>
#include <stdlib.h>
#include "../structures.h"
#include "../utils.h"
#include "ss_utils.h"

static ClosestPoints closestPair;

static int compare_x(const void* a, const void* c){
    Point *p1 = (Point *)a;
    Point *p2 = (Point *)c;
    return (p1->x - p2->x);
}

static int compare_y(const void* a, const void* c){
    Point *p1 = (Point *)a;
    Point *p2 = (Point *)c;
    return (p1->y - p2->y);
}

static double dist(Point p1, Point p2){
    return sqrt(squaredDistance(p1,p2));
}

static double brute_force(Point P[], int n){
    double min = INFINITY;
    for (int i = 0; i < n; ++i)
        for (int j = i+1; j < n; ++j)
            if (dist(P[i], P[j]) < min) {
                min = dist(P[i], P[j]);
                closestPair.point1 = P[i];
                closestPair.point2 = P[j];
                closestPair.distance = min;
            }
    return min;
}

static double min(double x, double y){
    return (x < y)? x : y;
}

static double strip_closest(Point strip[], int size, double d){
    double min = d;

    qsort(strip, size, sizeof(Point), compare_y);

    for (int i = 0; i < size; ++i)
        for (int j = i+1; j < size && (strip[j].y - strip[i].y) < min; ++j)
            if (dist(strip[i],strip[j]) < min) {
                min = dist(strip[i], strip[j]);
                closestPair.point1 = strip[i];
                closestPair.point2 = strip[j];
                closestPair.distance = min;
            }

    return min;
}

static double closest_util(Point P[], int n){
    if (n <= 3)
        return brute_force(P, n);

    int mid = n/2;
    Point midPoint = P[mid];

    double dl = closest_util(P, mid);
    double dr = closest_util(P + mid, n - mid);

    double d = min(dl, dr);

    Point strip[n];
    int j = 0;
    for (int i = 0; i < n; i++){
        double temp = P[i].x - midPoint.x;
        double abs = temp > 0 ? temp : temp*-1;
        if (abs < d)
            strip[j] = P[i], j++;
    }

    return min(d, strip_closest(strip, j, d) );
}

ClosestPoints closest(Point P[], int n){
    qsort(P, n, sizeof(Point), compare_x);

    closest_util(P, n);
    return closestPair;
}