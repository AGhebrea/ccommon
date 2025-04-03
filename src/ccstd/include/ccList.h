#pragma once

#include <stdlib.h>

typedef struct ccListNode{
    void* data;
    struct ccListNode* previous;
    struct ccListNode* next;
    void (*dtor_data)(void*);
}ccListNode_t;

typedef struct ccList{
    size_t size;
    ccListNode_t *head;
    ccListNode_t *tail;
}ccList_t;

ccList_t* ctor_ccList(void);
void dtor_ccList(ccList_t *list);

ccListNode_t* ctor_ccListNode(void* data, void (*dtor_data_fn)(void*));
void dtor_ccListNode(ccListNode_t* node);

void append_ccList(ccList_t* list, ccListNode_t* node);
void insert_ccList(ccList_t* list, size_t index, ccListNode_t* node);
void prepend_ccList(ccList_t* list, ccListNode_t* node);
void* itemAt_ccList(ccList_t* list, size_t index);
void delete_tail_ccList(ccList_t* list);
void delete_item_ccList(ccList_t* list, size_t index);
void delete_head_ccList(ccList_t* list);