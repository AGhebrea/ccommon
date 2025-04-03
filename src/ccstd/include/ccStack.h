#pragma once

#include <stdlib.h>

typedef struct ccStack{
    size_t size;
    size_t capacity;
    void** data;
    void (*dtor_data_fn)(void*);
}ccStack_t;

ccStack_t* ctor_ccStack(size_t capacity, void (*dtor_data_fn)(void*));
void dtor_ccStack(ccStack_t* ccStack);

void push_ccStack(ccStack_t* ccStack, void* data);
void* pop_ccStack(ccStack_t* ccStack);
void* peek_ccStack(ccStack_t* ccStack);