
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "./include/ccDynamicArray.h"
#include "../cclog/include/cclog_macros.h"

ccDynamicArray_t* ccDynamicArray_ctor(size_t itemSize, bool zeroMem)
{
    ccDynamicArray_t* newArray = NULL;

    expectExit(newArray, malloc(sizeof(ccDynamicArray_t)), != NULL);
    newArray->itemSize = itemSize;
    newArray->capacity = 4096;
    if(zeroMem)
        expectExit(newArray->data, calloc(newArray->capacity,  sizeof(char)), != NULL);
    else
        expectExit(newArray->data, malloc(newArray->capacity * sizeof(char)), != NULL);

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
    if(index > array->capacity)
        return NULL;
    return (array->data + (index * array->itemSize));
}

void ccDynamicArray_set(ccDynamicArray_t* array, size_t index, void* data)
{
    void* aux = NULL;
    size_t i = index * array->itemSize;

    while(i >= array->capacity){
        array->capacity = array->capacity * 2;
        if(i < array->capacity){
            array->data = realloc(array->data, array->capacity * sizeof(char));
        }
    }
    aux = array->data + i;
    memcpy(aux, data, array->itemSize);
}

void ccDynamicArray_alloc(ccDynamicArray_t* array, size_t size)
{
    while(size >= array->capacity){
        array->capacity = array->capacity * 2;
        if(size < array->capacity)
            array->data = realloc(array->data, array->capacity * sizeof(char));
    }
}