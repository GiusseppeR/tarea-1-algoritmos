#include <stdio.h>
#include <conio.h>
#include <time.h>
#include "structures.h"
#include "arrays/array.h"
#include "random/custom_random.h"
#include "utils.h"

int main(void) {
    Allocator allocator = {my_alloc, my_free, 0};
    splitmix64_init((uint64_t) time(NULL));
    xoshiro256plus_init();

    int n = 33554432;

    Point *input = array(Point, &allocator);

    for(int i = 0; i < n; i++)
        array_append(input, generateRandomPoint());

    printf("%i random points of double precision generated. \n",n);

    Tree * tree = ciaccia_patella(input);

    printf("Tree height: %i\n",tree->height);
    for(int i = 0; i < tree->size; i++)
        printf("radius %i: %f\n", i, tree->entries[i].radius);
    free_tree(tree);
    _getch();
    return 0;
}