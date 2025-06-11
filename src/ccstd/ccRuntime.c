
#include "include/ccRuntime.h"
#include "include/ccRBTree.h"
#include "include/ccDynamicArray.h"
#include "../cclog/include/cclog.h"
#include <string.h>

// TODO: add a way to turn off all of this runtime stuff if we're not in a debug build.
// TODO: add a way to efficiently do typechecking at runtime.

int assertType(void* data, char* name)
{
    ccType_t* type = (ccType_t*)data;
    char* typeName = type->name;
    return (strcmp(typeName, name) == 0);
}

void ccType_ctor(ccType_t* type, char* name)
{
    type->name = name;
}

void ccType_dtor(ccType_t* type)
{
    free(type);
}