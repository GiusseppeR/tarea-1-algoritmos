#include <stdio.h>
#include <time.h>
#include "structures.h"
#include "arrays/array.h"
#include "random/custom_random.h"
#include "utils.h"
#include "query/query.h"

int main(void) {
    Allocator allocator = {my_alloc, my_free, 0};
    splitmix64_init((uint64_t) time(NULL));
    xoshiro256plus_init();

    generate_experiment();

    return 0;
}