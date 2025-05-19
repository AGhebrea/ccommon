#pragma once 

typedef enum {
    black,
    red
}ccSetcolor_t;

typedef struct ccSetNode ccSetNode_t;
struct ccSetNode{
    ccSetcolor_t color;
    void* item;
    ccSetNode_t* left;
    ccSetNode_t* right;
    ccSetNode_t* parent;
};

typedef struct ccSet{
    ccSetNode_t* head;
    int (*compare_fn)(void*, void*);
    void (*dtor_data_fn)(void*);
}ccSet_t;

ccSet_t* ccSet_ctor(void (*dtor_data_fn)(void*), int (*compare_fn)(void*, void*));
void ccSet_dtor(ccSet_t *set);

ccSetNode_t* ccSetNode_ctor(void* data);
void ccSetNode_dtor(ccSetNode_t* node);

void ccSet_insert(ccSet_t* set, ccSetNode_t* node);
ccSetNode_t* ccSet_find(ccSet_t* set, void* data);

void dbg_printSet(ccSet_t* set);