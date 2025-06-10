#include <stdio.h>
#include <stdlib.h>
#include "../src/ccstd/include/ccRBTree.h"
#include "../src/ccstd/include/ccList.h"
#include "./include/test_ccRBTree.h"
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

int ccstd_ccRBTree_smoketest(void)
{
    #define ADD(val) tmp = malloc(sizeof(int));             \
        *tmp = val;                                         \
        ccRBTree_insert(set, ccRBTreeNode_ctor(tmp, tmp, NULL, NULL));
    #define DEL(val) tmp = malloc(sizeof(int)); \
        *tmp = val;                             \
        ccRBTree_remove(set, tmp);

    int status = 0;

    ccLog_setLogLevel(ccLogLevels_Info);

    int *tmp = NULL;
    ccRBTree_t* set = ccRBTree_ctor(&compareFn);

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

    if(found && *(int*)found->key == 15){
        status = 0;
    }else{
        status = 1;
    }

    #undef ADD
    #undef DEL

    return status;
}

void printNode(void* data)
{
    ccRBTreeNode_t* node = (ccRBTreeNode_t*)data;
    ccLogDebug("node->key: %c", *(char*)(node->key));
    ccLogDebug("node->data: %d", *(int*)(node->data));
}

int ccstd_ccRBTreeKeyed_smoketest()
{
    #define ADD(val, key) tmp = malloc(sizeof(int));            \
        keytmp = malloc(sizeof(char));                          \
        *tmp = val;                                             \
        *keytmp = key;                                          \
        ccRBTree_insert(set, ccRBTreeNode_ctor(tmp, keytmp, NULL, NULL));
    #define DEL(val) tmp = malloc(sizeof(int)); \
        *tmp = val;                             \
        ccRBTree_remove(set, tmp);

    int status = 0;

    ccLog_setLogLevel(ccLogLevels_Info);

    int *tmp = NULL;
    char* keytmp = NULL;
    ccRBTree_t* set = ccRBTree_ctor(&compareFn);

    ADD(11, 'a');
    ADD(2, 'b');
    ADD(14, 'c');
    ADD(1, 'a');
    ADD(7, 'b');
    ADD(5, 'c');
    ADD(8, 'a');
    ADD(15, 'b');
    ADD(4, 'c');

    DEL('a');

    tmp = malloc(sizeof(int));
    *tmp = 15;
    *keytmp = 'a';
    ccRBTreeNode_t* found = ccRBTree_find(set, keytmp);

    if(found && *(int*)found->data == 8){
        status = 0;
    }else{
        status = 1;
    }

    #undef ADD
    #undef DEL

    return status;
}

void printNodeList(void *data)
{
    ccList_t* list = *(ccList_t**)((ccRBTreeNode_t*)data)->key;
    ccListNode_t* node = NULL;

    ccLogDebug("Link START");
    ccLogDebug("List with key: %c", *(char*)((ccRBTreeNode_t*)data)->key);
    for(size_t i = 0; i < list->size; ++i){
        node = ccList_nodeAt(list, i);
        ccLogDebug(" %d", node->data);
    }
    ccLogDebug("Link END");
}

void addItem(ccRBTree_t* set, int data, char key)
{
    int *tmp = NULL;
    char* keytmp = NULL;
    tmp = malloc(sizeof(int));
    *tmp = data;
    keytmp = malloc(sizeof(char));
    *keytmp = key;
    ccRBTreeNode_t* auxnode;

    ccList_t* list = NULL;
    auxnode = ccRBTree_find(set, keytmp);
    if(auxnode != NULL){
        list = (ccList_t*)(auxnode->key);
    }else{
        list = ccList_ctor();
        ccRBTree_insert(set, ccRBTreeNode_ctor(&list, keytmp, NULL, NULL)); 
    }
    ccList_append(list, ccListNode_ctor(tmp, NULL));
    dbg_printSet(set, printNodeList);
}

void delItem(char key)
{

}

int ccstd_ccRBTreeKeyedNested_smoketest()
{
    int status = 0;
    ccLogLevel_t oldLogLevel = ccLog_getActiveLogLevel();

    ccLog_setLogLevel(ccLogLevels_Debug);

    ccRBTree_t* set = ccRBTree_ctor(&compareFn);

    addItem(set, 11, 'a');
    addItem(set, 2, 'b');
    addItem(set, 14, 'c');
    addItem(set, 1, 'a');
    addItem(set, 7, 'b');
    addItem(set, 5, 'c');
    addItem(set, 8, 'a');
    addItem(set, 15, 'b');
    addItem(set, 4, 'c');

    dbg_printSet(set, printNodeList);

    #undef ADD
    #undef DEL

    ccLog_setLogLevel(oldLogLevel);

    return status;
}

// // todo: add these tests for rbtree
// if(set->head->parent != set->null){
//     ccLogError("set->head->parent != set->null");
//     // exit(1);
// }
// double_t height = recurseHeight(set, set->head) - 1;
// double_t lg = 2 * log2(set->size + 1);
// if(height > lg){
//     ccLogError("Height too great %lf, size: %ld, lg: %lf", height, set->size, lg);
//     // exit(1);
// }else{
//     // ccLogError("Height: %lf, size: %ld, lg: %lf", height, set->size, lg);
// }
// if(recurseCheckFail(set, set->head) == 1)
// {
//     ccLogError("Tree is already corrupted");
//     // exit(1);
// }
// ccList_t* list = ccList_ctor();
// if(recurseCheckDuplicates(set, list, set->head) == 1)
// {
//     ccLogError("Tree has duplicates");
//     // exit(1);
// }
// list = ccList_ctor();
// void* a;
// void* b;
// for(size_t i = 0; i < list->size; ++i){
//     for(size_t j = i + 1; j < list->size; ++j){
//         a = ccList_nodeAt(list, i)->data;
//         b = ccList_nodeAt(list, j)->data;
//         if(a == b){
//             ccLogError("We somehow pushed the same data twice");
//             // exit(1);
//         }
//     }
// }
// int recurseCheckFail(ccRBTree_t* set, ccRBTreeNode_t* node)
// {
//     if(node == set->null)
//         return 0;

//     if(recurseCheckFail(set, node->left) == 1)
//         return 1;

//     // if((*(size_t*)((size_t)node - 8) & 1) != 0x1)
//     //     return 1;

//     return recurseCheckFail(set, node->right);
// }

// int recurseCheckDuplicates(ccRBTree_t* set, ccList_t* list, ccRBTreeNode_t* node)
// {
//     if(node == set->null)
//         return 0;

//     if(recurseCheckDuplicates(set, list, node->left) == 1)
//         return 1;

//     for(size_t i = 0; i < list->size; ++i){
//         if(node == ccList_nodeAt(list, i)->data){
//             return 1;
//         }
//     }

//     ccList_append(list, ccListNode_ctor(node, NULL));

//     return recurseCheckDuplicates(set, list, node->right);
// }

// double_t recurseHeight(ccRBTree_t *set, ccRBTreeNode_t* node)
// {
//     double_t left = 0;
//     double_t right = 0;

//     if(node == set->null)
//         return 0;

//     left = recurseHeight(set, node->left);
//     right = recurseHeight(set, node->right);

//     if(left > right)
//         return left+1;

//     return right+1;
// }
