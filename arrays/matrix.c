#include "matrix.h"

void **matrix_init(size_t item_size, size_t pointerSize, size_t capacity, int rowNumber, Allocator *a) {
    void **ptr = array_init(pointerSize,capacity, a);
    for (int i = 0; i < rowNumber; i++){
        void * row = array_init(item_size,capacity,a);
        array_append(ptr,row);
    }
    return ptr;
}
