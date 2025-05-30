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
    ccListNode_t* last;
    size_t lastIndex;
}ccList_t;

ccList_t* ccList_ctor(void);
void ccList_dtor(ccList_t *list);

ccListNode_t* ccListNode_ctor(void* data, void (*dtor_data_fn)(void*));
void ccListNode_dtor(ccListNode_t* node);

void ccList_append(ccList_t* list, ccListNode_t* node);
void ccList_insert(ccList_t* list, size_t index, ccListNode_t* node);
void ccList_prepend(ccList_t* list, ccListNode_t* node);
void* ccList_itemAt(ccList_t* list, size_t index);
void ccList_deleteTail(ccList_t* list);
void ccList_deleteItem(ccList_t* list, size_t index);
void ccList_deleteHead(ccList_t* list);
void ccList_delete(ccList_t* list, ccListNode_t* node);
void ccList_join(ccList_t* a, ccList_t* b);