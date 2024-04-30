
#ifndef TAREALOG_MATRIX_H
#define TAREALOG_MATRIX_H

#include "array.h"

#define matrix(T, rowNumber, a) (T**) matrix_init(sizeof(T), rowNumber*sizeof(T*) ,ARRAY_INITIAL_CAPACITY, rowNumber,a)
#define matrix_append(matrix, row, element) do{ \
    array_append(matrix[row], element);\
}while(0)

#define matrix_remove_row(x, l) do { \
    array_remove(x,l);               \
}while(0)
#define matrix_free(matrix) do { \
    for(int i = 0; i < array_length(matrix); i++) \
        array_free(matrix[i]);   \
    array_free(matrix);          \
}while(0)
 void **matrix_init(size_t item_size, size_t pointerSize, size_t capacity, int rowNumber, Allocator *a);

#endif
