#ifndef TAREALOG_2DTREES_H
#define TAREALOG_2DTREES_H

#include "../structures.h"
typedef struct btree2d BTree2D;

typedef struct btree2d{
    Point * point;
    BTree2D * left;
    BTree2D * right;
    int depth;
}BTree2D;

BTree2D *initBT(int depth);
int searchBT2D(BTree2D *tree, Point value);
void findClosestBT2D(BTree2D *tree, Point value, double bestDistance, Point *closest, int consider_self);
void findFarthestBT2D(BTree2D *tree, Point value, double bestDistance, Point *farthest);
void insertBT2D(BTree2D *tree, Point value);
void freeBT2D(BTree2D *tree);

#endif
