#pragma once

#include<stdlib.h>

typedef struct ccType{
    char* name;
}ccType_t;

void ccStdInit(void);
void ccType_ctor(ccType_t* type, char* name);
void ccType_dtor(ccType_t* type);