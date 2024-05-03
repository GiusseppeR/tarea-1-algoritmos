#ifndef TAREALOG_STRUCTURES_H
#define TAREALOG_STRUCTURES_H
#include "arrays/array.h"

#define B 4096
#define b 2048
#define TRUE 1
#define FALSE 0

typedef struct entry Entry;

typedef struct point{
    double x;
    double y;
    int index;
} Point;

typedef struct node{
    int height;
    int size;
    Entry *entries;
    Entry *parent;
}Tree;

typedef struct entry{
    Point point;
    double radius;
    Tree* subTree;
}Entry;
typedef struct closestPoints {
    Point point1;
    Point point2;
    float distance;
}ClosestPoints;
Tree * ciaccia_patella(Point * input);
void free_tree(Tree *tree);

#endif
