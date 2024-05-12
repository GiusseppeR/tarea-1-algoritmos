#include <stdlib.h>
#include <math.h>
#include "../utils.h"
#include "2dTrees.h"

BTree2D *initBT(int depth){
    BTree2D *output = malloc(sizeof(BTree2D));
    output->point = NULL;
    output->left = NULL;
    output->right = NULL;
    output->depth = depth;
    return output;
}

int searchBT2D(BTree2D *tree, Point value) {
    if  (tree->point == NULL)
        return 0;

    Point point = *(tree->point);

    if ((point.x == value.x) && (point.y == value.y))
        return 1;

    if (tree->depth % 2 == 1) {
        if (point.x < value.x)
            return searchBT2D(tree->left, value);
        else
            return searchBT2D(tree->right, value);
    } else {
        if (point.y < value.y)
            return searchBT2D(tree->left, value);
        else
            return searchBT2D(tree->right, value);
    }
}

void findClosestBT2D(BTree2D *tree, Point value, double bestDistance, Point *closest, int consider_self){
    if  (tree->point == NULL)
        return;

    Point point = *(tree->point);

    if ( consider_self == FALSE &&
    equals(value, point) == TRUE){
        findClosestBT2D(tree->left, value,bestDistance, closest, consider_self);
        findClosestBT2D(tree->right, value,bestDistance, closest, consider_self);
        return;
    }

    double distance = squaredDistance(value,point);

    if (distance < bestDistance)
        *closest = *tree->point;
    else
        distance = bestDistance;

    if (tree->depth % 2 == 1) {
        if (point.x < value.x)
            findClosestBT2D(tree->left, value, distance, closest, consider_self);
        else
            findClosestBT2D(tree->right, value, distance, closest, consider_self);
    } else {
        if (point.y < value.y)
            findClosestBT2D(tree->left, value, distance, closest, consider_self);
        else
            findClosestBT2D(tree->right, value, distance, closest, consider_self);
    }
}

void findFarthestBT2D(BTree2D *tree, Point value, double bestDistance, Point *farthest){
    if  (tree->point == NULL)
        return;

    Point point = *(tree->point);

    double distance = squaredDistance(value,point);

    if (distance > bestDistance)
        *farthest = *tree->point;
    else
        distance = bestDistance;

    if (tree->depth % 2 == 1) {
        if (point.x > value.x)
            findFarthestBT2D(tree->left, value, distance, farthest);
        else
            findFarthestBT2D(tree->right, value, distance, farthest);
    } else {
        if (point.y > value.y)
            findFarthestBT2D(tree->left, value, distance, farthest);
        else
            findFarthestBT2D(tree->right, value, distance, farthest);
    }
}

void insertBT2D(BTree2D *tree, Point value) {
    if (tree->point == NULL) {
        tree->point = malloc(sizeof(Point));
        *(tree->point) = value;
        tree->left = initBT(tree->depth + 1);
        tree->right = initBT(tree->depth + 1);
        return;
    }

    Point point = *(tree->point);

    if (tree->depth % 2 == 1) {
        if (point.x < value.x)
            insertBT2D(tree->left, value);
        else
            insertBT2D(tree->right, value);
    } else {
        if (point.y < value.y)
            insertBT2D(tree->left, value);
        else
            insertBT2D(tree->right, value);
    }
}

void freeBT2D(BTree2D * tree){
    if(tree == NULL)
        return;

    freeBT2D(tree->left);
    freeBT2D(tree->right);

    tree->left = NULL;
    tree->right = NULL;

    free(tree);
}