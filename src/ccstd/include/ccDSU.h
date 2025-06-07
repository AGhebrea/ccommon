#pragma once

#include "./ccstd.h"

typedef struct dsu{
    long* items;
    void* data;
    void (*dtor_data_fn)(void*);
}dsu_t;

dsu_t* dsu_ctor(void (*dtor_data_fn)(void*));
void dsu_dtor(dsu_t *set);