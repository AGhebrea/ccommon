
#include "./include/ccStack.h"
#include "../cclog/include/cclog_macros.h"

ccStack_t* ccStack_ctor(size_t capacity, void (*dtor_data_fn)(void*))
{
    ccStack_t* ret;

    expect(ret, malloc(sizeof(ccStack_t)), != NULL);
    expect(ret->data, malloc(sizeof(void**) * capacity), != NULL);
    ret->size = 0;
    ret->capacity = capacity;
    ret->dtor_data_fn = dtor_data_fn;

    return ret;
}

void ccStack_dtor(ccStack_t* ccStack)
{
    if(ccStack->dtor_data_fn != NULL){
        for(size_t i = 0; i < ccStack->size; ++i){
            ccStack->dtor_data_fn(&ccStack->data[i]);
        }
    }

    free(ccStack->data);
    free(ccStack);

    return;
}

void ccStack_push(ccStack_t* ccStack, void* data)
{
    ccStack->size++;
    
    if(ccStack->size >= ccStack->capacity){
        ccStack->capacity *= 2;
        expect(ccStack->data, realloc(ccStack->data, sizeof(void**) * ccStack->capacity), != NULL);
    }
    ccStack->data[ccStack->size] = data;

    return;
}

void* ccStack_pop(ccStack_t* ccStack)
{
    void* ret;

    if(ccStack->size == 0)
        return NULL;

    ret = ccStack->data[ccStack->size];
    ccStack->size--;

    return ret;
}

void* ccStack_peek(ccStack_t* ccStack)
{
    return ccStack->data[ccStack->size];
}