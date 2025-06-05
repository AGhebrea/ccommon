#pragma once

#include <stdlib.h>

typedef struct ccStack{
    size_t size;
    size_t capacity;
    void** data;
    void (*dtor_data_fn)(void*);
}ccStack_t;

ccStack_t* ccStack_ctor(size_t capacity, void (*dtor_data_fn)(void*));
void ccStack_dtor(ccStack_t* ccStack);

void ccStack_push(ccStack_t* ccStack, void* data);
void* ccStack_pop(ccStack_t* ccStack);
void* ccStack_peek(ccStack_t* ccStack);