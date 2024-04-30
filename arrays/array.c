#include <stdlib.h>
#include <string.h>
#include "array.h"

void *my_alloc(size_t bytes, void *context) {
    (void)context;
    return malloc(bytes);
}

void *my_free(size_t bytes, void *ptr, void *context) {
    (void)context;
    free(ptr);
}

void *array_init(size_t item_size, size_t capacity, Allocator *a) {
    void *ptr = 0;
    size_t size = item_size * capacity + sizeof(Array_Header);
    Array_Header *h = a->alloc(size, a->context);

    if (h) {
        h->capacity = capacity;
        h->length = 0;
        h->a = a;
        ptr = h + 1;
    }

    return ptr;
}

void *array_ensure_capacity(void *a, size_t item_count, size_t item_size) {
    Array_Header *h = array_header(a);
    size_t desired_capacity = h->length + item_count;

    if (h->capacity < desired_capacity) {
        size_t new_capacity = h->capacity * 2;
        while (new_capacity < desired_capacity) {
            new_capacity *= 2;
        }

        size_t new_size = sizeof(Array_Header) + new_capacity * item_size;
        Array_Header *new_h = h->a->alloc(new_size, h->a->context);

        if (new_h) {
            size_t old_size = sizeof(*h) + h->length * item_size;
            memcpy(new_h, h, old_size);

            if (h->a->free) {
                h->a->free(old_size, h, h->a->context);
            }

            new_h->capacity = new_capacity;
            h = new_h + 1;
        } else {
            h = 0;
        }
    } else { h += 1; }

    return h;
}