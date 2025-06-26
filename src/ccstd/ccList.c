#include "../cclog/include/cclog_macros.h"
#include "./include/ccList.h"

/* TODO: to do copy constructors we need to make the user specify a function to copy data.
 * at this point we should just use c++ */

ccList_t* ccList_ctor(void)
{
    ccList_t* newList;
    
    expect(newList, malloc(sizeof(ccList_t)), != NULL);

    newList->size = 0;
    newList->head = NULL;
    newList->tail = NULL;
    /* TODO: optimize a bit more the cache mechanism, at the moment we just
     * invalidate cache on each operation that changes list, we could be 
     * smarter about it but we also need to check if the optimization is
     * worth the added code complexity. */
    newList->last = NULL;
    newList->lastIndex = 0;

    ccType_ctor(&newList->type, "ccList_t");

    return newList;
}

void ccList_dtor(ccList_t *list)
{
    while(list->size > 0){
        ccList_deleteHead(list);
    }
    free(list);
}

ccListNode_t* ccListNode_ctor(void* data, void (*dtor_data_fn)(void*))
{
    ccListNode_t* newNode;

    expect(newNode, malloc(sizeof(ccListNode_t)), != NULL);
    newNode->data = data;
    newNode->dtor_data = dtor_data_fn;

    ccType_ctor(&newNode->type, "ccListNode_t");

    return newNode;
}

void ccListNode_dtor(ccListNode_t* node)
{
    if(node->dtor_data != NULL)
        node->dtor_data(node->data);
    free(node);
}

void ccList_append(ccList_t* list, ccListNode_t* node)
{
    node->next = NULL;

    list->last = NULL;

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

void ccList_insert(ccList_t* list, size_t index, ccListNode_t* node)
{
    ccListNode_t* tmp;

    list->last = NULL;

    if(index >= list->size - 1){
        ccLogDebug("index (%d) >= index of tail (%d); defaulting to append.", index, list->size - 1);
        ccList_append(list, node);
        return;
    }

    if(list->size == 0){
        ccLogDebug("index == 0; defaulting to prepend.");
        ccList_prepend(list, node);
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

void ccList_prepend(ccList_t* list, ccListNode_t* node)
{
    node->next = NULL;

    list->last = NULL;

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

ccListNode_t* ccList_nodeAt(ccList_t* list, size_t index)
{
    ccListNode_t* tmp = NULL;

    if(list->size == 0)
        return NULL;

    if(index > list->size){
        ccLogDebug("index (%ld) > list->size (%ld), returning tail element.");
        return list->tail->data;
    }

    if(list->last != NULL){
        if(list->lastIndex == index){
            tmp = list->last;
        }else if(list->lastIndex == index - 1){
            tmp = list->last->next;
        }else if(list->lastIndex == index + 1){
            tmp = list->last->previous;
        }
    }

    if(tmp == NULL){
        tmp = list->head;
        for(size_t i = 0; i < index; ++i)
            tmp = tmp->next;
    }

    list->last = tmp;
    list->lastIndex = index;

    return tmp;
}

void* ccList_itemAt(ccList_t* list, size_t index)
{
    ccListNode_t* node = ccList_nodeAt(list, index);

    if(node == NULL)
        return NULL;
    return node->data;
}

void ccList_deleteTail(ccList_t* list)
{
    ccListNode_t* tmp;

    list->last = NULL;

    if(list->size == 0){
        ccLogDebug("Tried to delete when size of list was 0! Nothing was done.");
        return;
    }

    switch(list->size){
    case 1:
        ccListNode_dtor(list->tail);
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

        ccListNode_dtor(list->tail);

        list->tail = tmp;

        list->size--;
        break;
    }

    return;
}

void ccList_deleteItem(ccList_t* list, size_t index)
{
    ccListNode_t* tmp;
    ccListNode_t* target;

    list->last = NULL;

    if(index >= list->size - 1){
        ccLogDebug("index (%d) >= index of tail (%d); defaulting to delete tail.", index, list->size - 1);
        ccList_deleteTail(list);
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

    ccListNode_dtor(target);

    list->size--;

    return;
}

void ccList_deleteHead(ccList_t* list)
{
    ccListNode_t* tmp;

    ccLogTrace("");

    list->last = NULL;

    if(list->size == 0){
        ccLogDebug("Tried to delete when size of list was 0! Nothing was done.");
        return;
    }

    switch(list->size){
    case 1:
        ccListNode_dtor(list->head);
        list->head = NULL;
        break;
    default:
        if(list->head->previous != NULL)
            ccLogError(": was somehow called on item which is not the head of the list.");

        tmp = list->head->next;
        tmp->previous = NULL;

        ccListNode_dtor(list->head);
        list->head = tmp;

        break;
    }

    list->size--;

    return;
}

void ccList_unlink(ccList_t* list, ccListNode_t* node)
{
    if(list->size == 0)
        return;

    list->last = NULL;

    if(node->previous != NULL){
        node->previous->next = node->next;
    }else{
        list->head = node->next;
    }
    if(node->next != NULL){
        node->next->previous = node->previous;
    }else{
        list->tail = node->previous;
    }

    node->previous = NULL;
    node->next = NULL;

    list->size -= 1;
}

void ccList_join(ccList_t* a, ccList_t* b)
{
    a->size += b->size;
    a->tail->next = b->head;
    b->head->previous = a->tail;
    a->tail = b->tail;
    b->last = NULL;
    a->last = NULL;
}