#include <stdio.h>
#include <stdlib.h>
#include "../src/ccstd/include/ccSet.h"
#include "./include/test_ccSet.h"
#include "../src/cclog/include/cclog.h"

int compareFn(void* a, void *b){
    if(*(int*)a > *(int*)b){
        return 1;
    }
    if(*(int*)a < *(int*)b){
        return -1;
    }
    return 0;
}

int ccstd_ccset_smoketest(void)
{
    #define ADD(val) tmp = malloc(sizeof(int));     \
        *tmp = val;                                 \
        ccSet_insert(set, ccSetNode_ctor(tmp));
    #define DEL(val) tmp = malloc(sizeof(int));     \
        *tmp = val;                                 \
        ccSet_remove(set, tmp);

    int status = 0;

    ccLog_setLogLevel(ccLogLevels_Info);

    int *tmp = NULL;
    ccSet_t* set = ccSet_ctor(NULL, &compareFn);

    ADD(11);
    ADD(2);
    ADD(14);
    ADD(1);
    ADD(7);
    ADD(5);
    ADD(8);
    ADD(15);
    ADD(4);

    dbg_printSet(set);

    DEL(11);
    dbg_printSet(set);

    DEL(14);
    dbg_printSet(set);

    tmp = malloc(sizeof(int));
    *tmp = 15;
    ccSetNode_t* found = ccSet_find(set, tmp);

    if(found && *(int*)found->item == 15){
        status = 0;
    }else{
        status = 1;
    }

    #undef ADD
    #undef DEL

    return status;
}