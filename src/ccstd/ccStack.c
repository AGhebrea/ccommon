
#include "./include/ccStack.h"
#include "../cclog/include/cclog_macros.h"

ccStack_t* ctor_ccStack(size_t capacity, void (*dtor_data_fn)(void*))
{
    ccStack_t* ret;

    expect(ret, malloc(sizeof(ccStack_t)), != NULL);
    expect(ret->data, malloc(sizeof(void**) * capacity), != NULL);
    ret->size = 0;
    ret->capacity = capacity;
    ret->dtor_data_fn = dtor_data_fn;

    return ret;
}

void dtor_ccStack(ccStack_t* ccStack)
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

void push_ccStack(ccStack_t* ccStack, void* data)
{
    size_t size;
    
    ccStack->size++;
    size = ccStack->size;
    
    if(size == ccStack->capacity){
        ccStack->capacity *= 2;
        expect(ccStack->data, realloc(ccStack->data, sizeof(void**) * ccStack->capacity), != NULL);
    }
    ccStack->data[size] = data;

    return;
}

void* pop_ccStack(ccStack_t* ccStack)
{
    ccStack_t* ret;

    if(ccStack->size == -1){
        return NULL;
    }else{
        ret = ccStack->data[ccStack->size];
        ccStack->size--;
    }

    return ret;
}

void* peek_ccStack(ccStack_t* ccStack)
{
    return ccStack->data[ccStack->size];
}