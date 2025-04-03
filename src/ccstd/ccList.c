#include "../cclog/include/cclog_macros.h"
#include "./include/ccList.h"

ccList_t* ctor_ccList(void)
{
    ccList_t* newList;
    
    expect(newList, malloc(sizeof(ccList_t)), != NULL);

    newList->size = 0;
    newList->head = NULL;
    newList->tail = NULL;

    return newList;
}

void dtor_ccList(ccList_t *list)
{
    while(list->size > 0){
        delete_head_ccList(list);
    }
    if(list != NULL){
        free(list);
    }
}

ccListNode_t* ctor_ccListNode(void* data, void (*dtor_data_fn)(void*))
{
    ccListNode_t* newNode;

    expect(newNode, malloc(sizeof(ccListNode_t)), != NULL);
    newNode->data = data;
    newNode->dtor_data = dtor_data_fn;

    return newNode;
}

void dtor_ccListNode(ccListNode_t* node)
{
    if(node->dtor_data != NULL)
        node->dtor_data(node->data);
    if(node != NULL)
        free(node);
}

void append_ccList(ccList_t* list, ccListNode_t* node)
{
    node->next = NULL;

    switch(list->size){
    case 0:
        list->head = node;
        node->previous = NULL;
        list->tail = node;
        break;
    default:
        list->tail->next = node;
        node->previous = list->tail;
        list->tail = node;
        break;
    }

    list->size++;

    return;
}

void insert_ccList(ccList_t* list, size_t index, ccListNode_t* node)
{
    ccListNode_t* tmp;

    if(index >= list->size - 1){
        ccLogDebug("index (%d) >= index of tail (%d); defaulting to append.", index, list->size - 1);
        append_ccList(list, node);
        return;
    }

    if(list->size == 0){
        ccLogDebug("index == 0; defaulting to prepend.");
        prepend_ccList(list, node);
        return;
    }

    if(index < list->size / 2){
        tmp = list->head;
        while(index){
            tmp = tmp->next;
            index--;
        }
    }else{
        tmp = list->tail;
        while(index){
            tmp = tmp->previous;
            index--;
        }
    }

    node->next = tmp->next;
    tmp->next->previous = node;
    node->previous = tmp;
    tmp->next = node;

    list->size++;

    return;
}

void prepend_ccList(ccList_t* list, ccListNode_t* node)
{
    node->next = NULL;

    switch(list->size){
    case 0:
        list->head = node;
        node->previous = NULL;
        list->tail = node;
        break;
    default:
        list->head->previous = node;
        node->next = list->head;
        list->head = node;
        break;
    }

    list->size++;

    return;
}

void* itemAt_ccList(ccList_t* list, size_t index)
{
    ccListNode_t* tmp;

    if(index > list->size){
        ccLogDebug("index (%ld) > list->size (%ld), returning tail element.");
        return list->tail->data;
    }

    tmp = list->head;
    for(size_t i = 0; i < index; ++i)
        tmp = tmp->next;

    return tmp->data;
}

void delete_tail_ccList(ccList_t* list)
{
    ccListNode_t* tmp;

    if(list->size == 0){
        ccLogDebug("Tried to delete when size of list was 0! Nothing was done.");
        return;
    }

    switch(list->size){
    case 1:
        dtor_ccListNode(list->tail);
        list->tail = NULL;
        break;
    default:
        /* todo: these are more debugging checks which should really be removed from finished product
         * it's not really worth it to even do this check this but i'll leave this here in order
         * to have an example for building a mechanism to omit parts of code that are strictly for
         * debugging. */
        if(list->tail->next != NULL)
            ccLogError(": was somehow called on item which is not the tail of the list.");

        tmp = list->tail->previous;
        tmp->next = NULL;

        dtor_ccListNode(list->tail);

        list->tail = tmp;

        list->size--;
        break;
    }

    return;
}

void delete_item_ccList(ccList_t* list, size_t index)
{
    ccListNode_t* tmp;
    ccListNode_t* target;

    if(index >= list->size - 1){
        ccLogDebug("index (%d) >= index of tail (%d); defaulting to delete tail.", index, list->size - 1);
        delete_tail_ccList(list);
        return;
    }

    if(list->size == 0){
        ccLogDebug("index == 0; defaulting to doing nothing.");
        return;
    }

    if(index < list->size / 2){
        target = list->head;
        while(index){
            target = target->next;
            index--;
        }
    }else{
        target = list->tail;
        while(index){
            target = target->previous;
            index--;
        }
    }

    tmp = target->previous;
    tmp->next = target->next;
    target->next->previous = tmp;

    dtor_ccListNode(target);

    list->size--;

    return;
}

void delete_head_ccList(ccList_t* list)
{
    ccListNode_t* tmp;

    ccLogTrace();

    if(list->size == 0){
        ccLogDebug("Tried to delete when size of list was 0! Nothing was done.");
        return;
    }

    switch(list->size){
    case 1:
        dtor_ccListNode(list->head);
        list->head = NULL;
        break;
    default:
        // todo: same as above
        if(list->head->previous != NULL)
            ccLogError(": was somehow called on item which is not the head of the list.");

        tmp = list->head->next;
        tmp->previous = NULL;

        dtor_ccListNode(list->head);
        list->head = tmp;

        break;
    }

    list->size--;

    return;
}