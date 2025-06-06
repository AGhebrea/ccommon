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
        ccRBTree_insert(set, ccRBTreeNode_ctor(tmp, NULL));
    #define DEL(val) tmp = malloc(sizeof(int)); \
        *tmp = val;                             \
        ccRBTree_remove(set, tmp);

    int status = 0;

    ccLog_setLogLevel(ccLogLevels_Info);

    int *tmp = NULL;
    ccRBTree_t* set = ccRBTree_ctor(0, NULL, &compareFn);

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

void printNode(void* data)
{
    ccRBTreeNode_t* node = (ccRBTreeNode_t*)data;
    ccLogDebug("node->key: %c", *(char*)(node->key));
    ccLogDebug("node->item: %d", *(int*)(node->item));
}

int ccstd_ccRBTreeKeyed_smoketest()
{
    #define ADD(val, key) tmp = malloc(sizeof(int));            \
        keytmp = malloc(sizeof(char));                          \
        *tmp = val;                                             \
        *keytmp = key;                                          \
        ccRBTree_insert(set, ccRBTreeNode_ctor(tmp, keytmp));
    #define DEL(val) tmp = malloc(sizeof(int)); \
        *tmp = val;                             \
        ccRBTree_remove(set, tmp);

    int status = 0;

    ccLog_setLogLevel(ccLogLevels_Info);

    int *tmp = NULL;
    char* keytmp = NULL;
    ccRBTree_t* set = ccRBTree_ctor(1, NULL, &compareFn);

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

    if(found && *(int*)found->item == 1){
        status = 0;
    }else{
        status = 1;
    }

    dbg_printSet(set, printNode);

    #undef ADD
    #undef DEL

    return status;
}

void printNodeList(void *data)
{
    ccList_t* list = *(ccList_t**)((ccRBTreeNode_t*)data)->item;
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
        list = (ccList_t*)(auxnode->item);
    }else{
        list = ccList_ctor();
        ccRBTree_insert(set, ccRBTreeNode_ctor(&list, keytmp)); 
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

    ccRBTree_t* set = ccRBTree_ctor(1, NULL, &compareFn);

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