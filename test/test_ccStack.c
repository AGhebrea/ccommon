
#include "../src/cclog/include/cclog_macros.h"
#include "../src/ccstd/include/ccStack.h"
#include "./include/test_ccStack.h"

int ccstd_ccstack_smoketest()
{
    ccStack_t* stack;
    int fail = 0;
    size_t* tmp;
    size_t data[4] = {4, 3, 2, 1};

    stack = ccStack_ctor(1024, NULL);

    for(size_t i = 0; i < 4; ++i){
        ccStack_push(stack, &data[i]);
        if(ccStack_peek(stack) != &data[i]){
            fail = 1;
            ccLogError("Peek stack is incorrect for index %ld!", i);
            return fail;
        }
    }

    for(size_t i = 4; i > 0; --i){
        tmp = (size_t*)ccStack_pop(stack);
        if(tmp != &data[i - 1]){
            fail = 1;
            ccLogError("Pop of stack is incorrect for index %ld!", i);
            return fail;
        }
    }

    if(stack->size != 0){
        fail = 1;
        ccLogError("Size of stack is not 0 at the end!");
        return fail;
    }

    ccStack_dtor(stack);

    return fail;
}