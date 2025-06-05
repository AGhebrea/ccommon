#include <stdio.h>
#include <stdlib.h>
#include "../src/ccstd/include/ccRBTree.h"
#include "./include/test_ccRBTree.h"
#include "../src/cclog/include/cclog.h"

struct cust{
    int i;
    char c;
};

void dbg_printSet(ccRBTree_t* set);

int compareFn(void* a, void *b){
    if(*(int*)a > *(int*)b){
        return 1;
    }
    if(*(int*)a < *(int*)b){
        return -1;
    }
    return 0;
}

int ccstd_ccRBTree_smoketest(void)
{
    #define ADD(val) tmp = malloc(sizeof(int));     \
        *tmp = val;                                 \
        ccRBTree_insert(set, ccRBTreeNode_ctor(tmp));
    #define DEL(val) tmp = malloc(sizeof(int));     \
        *tmp = val;                                 \
        ccRBTree_remove(set, tmp);

    int status = 0;

    ccLog_setLogLevel(ccLogLevels_Info);

    int *tmp = NULL;
    ccRBTree_t* set = ccRBTree_ctor(NULL, &compareFn);

    ADD(11);
    ADD(2);
    ADD(14);
    ADD(1);
    ADD(7);
    ADD(5);
    ADD(8);
    ADD(15);
    ADD(4);

    // dbg_printSet(set);

    DEL(11);
    // dbg_printSet(set);

    DEL(14);
    // dbg_printSet(set);

    tmp = malloc(sizeof(int));
    *tmp = 15;
    ccRBTreeNode_t* found = ccRBTree_find(set, tmp);

    if(found && *(int*)found->item == 15){
        status = 0;
    }else{
        status = 1;
    }

    #undef ADD
    #undef DEL

    return status;
}


void dbg_printSet1(struct cust* data, int* idx, ccRBTreeNode_t* node)
{
    if(node->left == NULL && node->right == NULL){
        data[*idx].i = *(int*)(node->item);
        if(node->color == red)
            data[*idx].c = 'r';
        else
            data[*idx].c = 'b';
        *idx += 1;
        return;
    }

    if(node->left != NULL)
        dbg_printSet1(data, idx, node->left);

    data[*idx].i = *(int*)node->item;    
    if(node->color == red)
        data[*idx].c = 'r';
    else
        data[*idx].c = 'b';
    *idx += 1;

    if(node->right != NULL)
        dbg_printSet1(data, idx, node->right);

    return;
}

void dbg_printSet(ccRBTree_t* set)
{
    ccRBTreeNode_t* node = set->head;
    struct cust data[100];
    int idx = 0;
    int* idxp = &idx;

    for(size_t i = 0; i < 100; ++i){
        data[i].i = 0;
    }

    dbg_printSet1(data, idxp, node);

    for(size_t i = 0; i < 100; ++i){
        if(data[i].i == 0)
            break;
        printf("%d%c ", data[i].i, data[i].c);
    }
    printf("\n");

    return;
}