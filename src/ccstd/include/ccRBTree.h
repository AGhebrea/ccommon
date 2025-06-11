#pragma once 

#include "./ccstd.h"

typedef enum {
    black,
    red
}ccRBTreecolor_t;

typedef struct ccRBTreeNode ccRBTreeNode_t;
struct ccRBTreeNode{
    ccType_t type;
    ccRBTreecolor_t color;
    void* key;
    void* data;
    void (*dtor_data_fn)(void*);
    void (*dtor_key_fn)(void*);
    ccRBTreeNode_t* left;
    ccRBTreeNode_t* right;
    ccRBTreeNode_t* parent;
};

typedef struct ccRBTree{
    ccType_t type;
    ccRBTreeNode_t* head;
    ccRBTreeNode_t* null;
    int (*compare_fn)(void*, void*);
    size_t size;
}ccRBTree_t;

ccRBTree_t* ccRBTree_ctor(int (*compare_fn)(void*, void*));
void ccRBTree_dtor(ccRBTree_t *set);

ccRBTreeNode_t* ccRBTreeNode_ctor(void* data, void* key, void (*dtor_data_fn)(void*), void (*dtor_key_fn)(void*));
void ccRBTreeNode_dtor(ccRBTreeNode_t* node);

void ccRBTree_insert(ccRBTree_t* set, ccRBTreeNode_t* node);
int ccRBTree_contains(ccRBTree_t* set, void* key);
ccRBTreeNode_t* ccRBTree_find(ccRBTree_t* set, void* key);
void ccRBTree_remove(ccRBTree_t* set, void* key);
void ccRBTree_removeNode(ccRBTree_t* set, ccRBTreeNode_t* node);
int ccRBTree_isEmpty(ccRBTree_t* set);
int ccRBTree_isLeaf(ccRBTree_t* set, ccRBTreeNode_t* node);

void dbg_printSet(ccRBTree_t* set, void (*printData)(void*));