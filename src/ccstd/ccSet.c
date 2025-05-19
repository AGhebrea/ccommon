#include <stdlib.h>
#include "./include/ccStack.h"
#include "./include/ccSet.h"
#include "../cclog/include/cclog_macros.h"

ccSet_t* ccSet_ctor(void (*dtor_data_fn)(void*), int (*compare_fn)(void*, void*))
{
    ccSet_t* set = malloc(sizeof(ccSet_t));
    set->dtor_data_fn = dtor_data_fn;
    set->compare_fn = compare_fn;
    set->head = NULL;

    return set;
}

void ccSet_dtor(ccSet_t *set)
{
    ccLogNotImplemented;
}

ccSetNode_t* ccSetNode_ctor(void* data)
{
    ccSetNode_t* node = malloc(sizeof(ccSetNode_t));
    node->color = red;
    node->item = data;
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;

    return node;
}

void ccSetNode_dtor(ccSetNode_t* node)
{
    ccLogNotImplemented;
}

static void leftr(ccSet_t* set, ccSetNode_t* node)
{
    ccSetNode_t* other = node->right;
    node->right = other->left;
    other->parent = node->parent;
    if(other->left != NULL)
        other->left->parent = node;
    if(node->parent != NULL){
        if(node->parent->left == node){
            node->parent->left = other;
        }else{
            node->parent->right = other;
        }
    }else{
        set->head = node;
    }
    node->parent = other;
    other->left = node;
}

static void rightr(ccSet_t* set, ccSetNode_t* node)
{
    ccSetNode_t* other = node->left;
    node->left = other->right;
    other->parent = node->parent;
    if(node->right != NULL)
        node->right->parent = other;
    if(node->parent != NULL){
        if(node->parent->left == node){
            node->parent->left = other;
        }else{
            node->parent->right = other;
        }
    }else{
        set->head = node;
    }
    node->parent = other;
    other->right = node;
}

static void insert(ccSet_t* set, ccSetNode_t* node, ccSetNode_t* where)
{
    if(set->head == NULL){
        set->head = node;
        return;
    }
    if(where == NULL){
        where = set->head;
    }
    if(set->compare_fn(node->item, where->item) > 0){
        if(where->right == NULL){
            node->parent = where;
            where->right = node;
        }else{
            insert(set, node, where->right);
        }
    }else{
        if(where->left == NULL){
            node->parent = where;
            where->left = node;
        }else{
            insert(set, node, where->left);
        }
    }
}

void ccSet_insert(ccSet_t* set, ccSetNode_t* node)
{
    ccSetNode_t* other;
    insert(set, node, NULL);
    /* color is already red from constructor */
    while(node != set->head && node->parent->color == red){
        if(node->parent->parent == NULL){
            node->color = black;
            return;
        }
        if(node->parent == node->parent->parent->left){
            other = node->parent->parent->right;
            if(other->color == red){
                node->parent->color = black;
                other->color = black;
                node->parent->parent->color = red;
                node = node->parent->parent;
            }else{
                if(node == node->parent->right){
                    node = node->parent;
                    leftr(set, node);
                }
                node->parent->color = black;
                node->parent->parent->color = red;
                rightr(set, node->parent->parent);
            }
        }else{
            other = node->parent->parent->left;
            if(other->color == red){
                node->parent->color = black;
                other->color = black;
                node->parent->parent->color = red;
                node = node->parent->parent;
            }else{
                if(node == node->parent->left){
                    node = node->parent;
                    // do we swap leftr and rightr as well ?
                    rightr(set, node);
                }
                node->parent->color = black;
                node->parent->parent->color = red;
                leftr(set, node->parent->parent);
            }
        }
    }
}

ccSetNode_t* ccSet_find(ccSet_t* set, void* data)
{
    ccSetNode_t* cursor = set->head;

    while(1){
        if(cursor == NULL)
            return NULL;
        if(set->compare_fn(cursor->item, data) == 0){
            return cursor;
        }else{
            if(set->compare_fn(cursor, data) > 0){
                cursor = cursor->left;
            }else{
                cursor = cursor->right;
            }
        }
    }
}

struct cust{
    int i;
    char c;
};

void dbg_printSet1(struct cust* data, int* idx, ccSetNode_t* node)
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

void dbg_printSet(ccSet_t* set)
{
    ccSetNode_t* node = set->head;
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