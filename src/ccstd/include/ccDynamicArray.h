#pragma once

#include <stddef.h>
#include <stdbool.h>

#include "./ccstd.h"
#include "ccRuntime.h"

typedef struct ccDynamicArray{
    ccType_t type;
    size_t size;
    size_t capacity;
    size_t itemSize;
    void* data;
    int zeroMem;
}ccDynamicArray_t;

ccDynamicArray_t* ccDynamicArray_ctor(size_t itemSize, bool zeroMem);
void ccDynamicArray_dtor(ccDynamicArray_t* array);

void* ccDynamicArray_get(ccDynamicArray_t* array, size_t index);
void ccDynamicArray_set(ccDynamicArray_t* array, size_t index, void* data);