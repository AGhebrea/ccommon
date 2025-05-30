#include "../src/cclog/include/cclog_macros.h"
#include "../src/ccstd/include/ccDynamicArray.h"

int ccstd_ccDynamicArray_smoketest()
{
    ccLogTrace();

    size_t tmp = 0;

    ccDynamicArray_t* array = ccDynamicArray_ctor(sizeof(size_t), 0);
    for(size_t i = 0; i < 4096 * 4; ++i)
        ccDynamicArray_set(array, i, &i);

    for(size_t i = 0; i < 4096 * 4; ++i){
        tmp = *(size_t*)ccDynamicArray_get(array, i);
        if(i != tmp){
            ccLogError("Error at index %ld!", i);
            return 1;
        }
    }

    return 0;
}