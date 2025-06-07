#pragma once

#include <stddef.h>
#include <stdbool.h>

#include "./ccstd.h"

typedef struct ccDynamicArray{
    size_t size;
    size_t capacity;
    size_t itemSize;
    void* data;
}ccDynamicArray_t;

ccDynamicArray_t* ccDynamicArray_ctor(size_t itemSize, bool zeroMem);
void ccDynamicArray_dtor(ccDynamicArray_t* array);

void* ccDynamicArray_get(ccDynamicArray_t* array, size_t index);
void ccDynamicArray_set(ccDynamicArray_t* array, size_t index, void* data);
void ccDynamicArray_alloc(ccDynamicArray_t* array, size_t size);