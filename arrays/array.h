#pragma once
#ifndef TAREALOG_ARRAY_H
#define TAREALOG_ARRAY_H

#include<stdio.h>
#include <string.h>

#define ARRAY_INITIAL_CAPACITY 16

#define array(T, a) array_init(sizeof(T), ARRAY_INITIAL_CAPACITY, a)

#define array_append(a, v) ( \
    (a) = array_ensure_capacity(a, 1, sizeof(v)), \
    (a)[array_header(a)->length] = (v), \
    &(a)[array_header(a)->length++])

#define array_merge(array1,array2) do { \
    for(int x = 0; x < array_length(array2); x++) \
        array_append(array1,array2[x]);           \
}while(0)

#define array_header(a) ((Array_Header *)(a) - 1)

#define array_length(a) (array_header(a)->length)
#define array_free(array) (array_header(array)->a->free(array_header(array)->capacity*sizeof(*array),array_header(array), array_header(array)->a->context))
#define array_capacity(a) (array_header(a)->capacity)

#define array_remove(a, i) do { \
    Array_Header *h = array_header(a); \
    if (i == h->length - 1) { \
        h->length -= 1; \
    } else if (h->length > 1) { \
        void *ptr = &a[i]; \
        void *post = &a[i + 1]; \
        h->length -= 1; \
        memcpy(ptr, post, (h->length - i) * sizeof(*a)); \
    } \
} while(0)

#define array_replace(array, index, elements) do { \
    Array_Header *h1 = array_header(array);        \
    Array_Header *h2 = array_header(elements); \
    array_ensure_capacity(array, h2->length, sizeof(*elements)); \
    void *ptr = &array[index];                      \
    void *post = &array[index + 1];\
    h1->length += h2->length - 1;                   \
    memcpy(post + (h2->length - 1)*sizeof(*array), post , (h1->length - index - 1)*sizeof(*array)); \
    memcpy(ptr, elements, (h2->length)*sizeof(*array));          \
    }while(0)

typedef struct {
    void *(*alloc)(size_t bytes, void *context);
    void *(*free)(size_t bytes, void *ptr, void *context);
    void *context;
} Allocator;

typedef struct {
    size_t length;
    size_t capacity;
    size_t padding;
    Allocator *a;
} Array_Header;

void *my_alloc(size_t bytes, void *context);
void *my_free(size_t bytes, void *ptr, void *context);
void *array_init(size_t item_size, size_t capacity, Allocator *a);
void *array_ensure_capacity(void *a, size_t item_count, size_t item_size);

#endif
