#pragma once 

typedef enum {
    black,
    red
}ccRBTreecolor_t;

typedef struct ccRBTreeNode ccRBTreeNode_t;
struct ccRBTreeNode{
    ccRBTreecolor_t color;
    void* item;
    ccRBTreeNode_t* left;
    ccRBTreeNode_t* right;
    ccRBTreeNode_t* parent;
};

typedef struct ccRBTree{
    ccRBTreeNode_t* head;
    int (*compare_fn)(void*, void*);
    void (*dtor_data_fn)(void*);
}ccRBTree_t;

ccRBTree_t* ccRBTree_ctor(void (*dtor_data_fn)(void*), int (*compare_fn)(void*, void*));
void ccRBTree_dtor(ccRBTree_t *set);

ccRBTreeNode_t* ccRBTreeNode_ctor(void* data);
void ccRBTreeNode_dtor(ccRBTreeNode_t* node);

void ccRBTree_insert(ccRBTree_t* set, ccRBTreeNode_t* node);
ccRBTreeNode_t* ccRBTree_find(ccRBTree_t* set, void* data);
void ccRBTree_remove(ccRBTree_t* set, void* data);