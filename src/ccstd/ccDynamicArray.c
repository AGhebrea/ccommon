
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "./include/ccDynamicArray.h"
#include "../cclog/include/cclog_macros.h"
#include "include/ccRuntime.h"

static inline void ccDynamicArray_alloc(ccDynamicArray_t* array, size_t index);

ccDynamicArray_t* ccDynamicArray_ctor(size_t itemSize, bool zeroMem)
{
    ccDynamicArray_t* newArray = NULL;

    expectExit(newArray, malloc(sizeof(ccDynamicArray_t)), != NULL);
    newArray->itemSize = itemSize;
    newArray->capacity = 4096;
    newArray->size = 0;
    if(zeroMem)
        expectExit(newArray->data, calloc(newArray->capacity,  sizeof(char)), != NULL);
    else
        expectExit(newArray->data, malloc(newArray->capacity * sizeof(char)), != NULL);
    newArray->zeroMem = zeroMem;

    ccType_ctor(&newArray->type, "ccDynamicArray_t");

    return newArray;
}

void ccDynamicArray_dtor(ccDynamicArray_t* array)
{
    if(array != NULL)
        free(array->data);
    free(array);
}

void* ccDynamicArray_get(ccDynamicArray_t* array, size_t index)
{
    size_t i = index * array->itemSize;

    ccDynamicArray_alloc(array, index);

    return (array->data + i);
}

void ccDynamicArray_set(ccDynamicArray_t* array, size_t index, void* data)
{
    void* aux = NULL;
    size_t i = index * array->itemSize;

    ccDynamicArray_alloc(array, index);
    
    aux = array->data + i;
    memcpy(aux, data, array->itemSize);
}

static inline void ccDynamicArray_alloc(ccDynamicArray_t* array, size_t index)
{
    size_t i = index * array->itemSize;
    size_t oldcap = array->capacity;

    if(index >= array->size)
        array->size = index + 1;
    while(i >= array->capacity){
        array->capacity = array->capacity * 2;
        if(i < array->capacity){
            array->data = realloc(array->data, array->capacity * sizeof(char));
            if(array->zeroMem)
                memset(array->data + oldcap, 0, array->capacity - oldcap);
        }
    }
}