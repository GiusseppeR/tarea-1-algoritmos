#include "utils.h"
#include "random/custom_random.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <stdlib.h>

//meter al utils
int compare(Point p1, Point p2) {
    return (p1.x == p2.x) & (p1.y == p2.y);
}
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

// Global variable to store the closest pair of points
ClosestPoints closestPair;

/* Following two functions are needed for library function qsort().
   Refer: http://www.cplusplus.com/reference/clibrary/cstdlib/qsort/ */

// Needed to sort array of points according to X coordinate
int compareX(const void* a, const void* c)
{
    Point *p1 = (Point *)a,  *p2 = (Point *)c;
    return (p1->x - p2->x);
}

// Needed to sort array of points according to Y coordinate
int compareY(const void* a, const void* c)
{
    Point *p1 = (Point *)a,   *p2 = (Point *)c;
    return (p1->y - p2->y);
}

// A utility function to find the distance between two points
float dist(Point p1, Point p2)
{
    return sqrt( (p1.x - p2.x)*(p1.x - p2.x) +
                 (p1.y - p2.y)*(p1.y - p2.y)
               );
}

// A Brute Force method to return the smallest distance between two points
// in P[] of size n
float bruteForce(Point P[], int n)
{
    float min = FLT_MAX;
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

// A utility function to find a minimum of two float values
float min(float x, float y)
{
    return (x < y)? x : y;
}


// A utility function to find the distance between the closest points of
// strip of a given size. All points in strip[] are sorted according to
// y coordinate. They all have an upper bound on minimum distance as d.
// Note that this method seems to be a O(n^2) method, but it's a O(n)
// method as the inner loop runs at most 6 times
float stripClosest(Point strip[], int size, float d)
{
    float min = d;  // Initialize the minimum distance as d

    qsort(strip, size, sizeof(Point), compareY);

    // Pick all points one by one and try the next points till the difference
    // between y coordinates is smaller than d.
    // This is a proven fact that this loop runs at most 6 times
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

// A recursive function to find the smallest distance. The array P contains
// all points sorted according to x coordinate
float closestUtil(Point P[], int n)
{
    // If there are 2 or 3 points, then use brute force
    if (n <= 3)
        return bruteForce(P, n);

    // Find the middle point
    int mid = n/2;
    Point midPoint = P[mid];

    // Consider the vertical line passing through the middle point
    // calculate the smallest distance dl on left of middle point and
    // dr on right side
    float dl = closestUtil(P, mid);
    float dr = closestUtil(P + mid, n-mid);

    // Find the smaller of two distances
    float d = min(dl, dr);

    // Build an array strip[] that contains points close (closer than d)
    // to the line passing through the middle point
    Point strip[n];
    int j = 0;
    for (int i = 0; i < n; i++)
        if (abs(P[i].x - midPoint.x) < d)
            strip[j] = P[i], j++;

    // Find the closest points in strip.  Return the minimum of d and closest
    // distance is strip[]
    return min(d, stripClosest(strip, j, d) );
}

// The main function that finds the smallest distance
// This method mainly uses closestUtil()
ClosestPoints closest(Point P[], int n){
    qsort(P, n, sizeof(Point), compareX);

    // Use recursive function closestUtil() to find the smallest distance
    closestUtil(P, n);
    return closestPair;
}

void show_tree(Tree* tree_ss) {
    printf("Dentro de showfree!\n");
    if (tree_ss == NULL) {
        printf("Lol\n");
        return;
    }
    printf("Tree size: %d y tree largo de entries: %lu\n",tree_ss->size,array_length(tree_ss->entries));
    for (int i = 0; i<tree_ss->size;i++) {
        printf("Entry %d:\n", i + 1);
        printf("Point: (%f, %f)\n", tree_ss->entries[i].point.x, tree_ss->entries[i].point.y);
        printf("Radius: %.2f\n", tree_ss->entries[i].radius);
        // Recursivamente imprimimos las entradas del subárbol
        show_tree(tree_ss->entries[i].subTree);
    }

}