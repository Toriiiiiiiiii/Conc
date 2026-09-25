#ifndef DA_H
#define DA_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * DYNAMIC ARRAYS
 */

typedef struct {
    uint64_t size;    // Size of a single member
    uint64_t nmem;    // Number of elements
    uint64_t maxmem;  // Maximum number of elements
    void*    arrbase; // Base of the array
} dynamic_array_t;

// Dynamic arrays will make use of macros to be generic.

#define da_create(t) (dynamic_array_t){\
        .size = sizeof(t),\
        .nmem = 0,\
        .maxmem = 2,\
        .arrbase = malloc(2 * sizeof(t))\
    }

#define da_destroy(arr)\
    arr.size = 0;\
    arr.nmem = 0;\
    arr.maxmem = 0;\
    free(arr.arrbase);

// da_peek: Get the element `i` from dynamic array `arr`, with data type `t`.
#define da_peek(arr, i, t) (*((t*)((uintptr_t)arr.arrbase + arr.size * i)))

#define da_set(arr, i, v, t) __extension__ ({\
    t temp = v;\
    memcpy((void*)((uintptr_t)arr.arrbase + arr.size * i), &temp, arr.size);})

// da_push: Append `v` to dynamic array `arr`
#define da_push(arr, v, t) __extension__ ({\
    da_set(arr, arr.nmem, v, t);\
    arr.nmem++; \
    if(arr.nmem == arr.maxmem) { \
        arr.arrbase = realloc(arr.arrbase, arr.maxmem * arr.size * 2); \
        arr.maxmem *= 2; \
    } arr.nmem-1; })

// da_pop: Pop the last element in dynamic array `arr`
#define da_pop(arr, t) __extension__ ({\
    arr.nmem--; \
    t retval = da_peek(arr, arr.nmem, t);\
    if(arr.nmem < arr.maxmem / 2) { \
        arr.arrbase = realloc(arr.arrbase, arr.maxmem * arr.size / 2); \
        arr.maxmem /= 2; \
    } retval; })

#define da_remove(arr, i, t) __extension__ ({\
    size_t tomove = arr.size * (arr.nmem - i - 1); \
    t retval = da_peek(arr, i, t); \
    memmove(arr.arrbase + arr.size * (i), arr.arrbase + arr.size * (i+1), tomove);\
    da_pop(arr, t); \
    retval; })

#define da_insert(arr, i, v, t) __extension__ ({\
    da_push(arr, v, t);\
    size_t tomove = arr.size * (arr.nmem - i);\
    memmove(arr.arrbase + arr.size * (i+1), arr.arrbase + arr.size * (i), tomove);\
    da_set(arr, i, v, t); })

#endif // DA_H
