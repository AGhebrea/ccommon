#include <stdlib.h>
#include "./include/ccRBTree.h"
#include "../cclog/include/cclog_macros.h"
#include "include/ccStack.h"

int compareImpl(ccRBTree_t* set, ccRBTreeNode_t* node, void* data);
int compareInsertImpl(ccRBTree_t* set, ccRBTreeNode_t* node, ccRBTreeNode_t* where);

ccRBTree_t* ccRBTree_ctor(int keyed, int (*compare_fn)(void*, void*))
{
    ccRBTree_t* set = malloc(sizeof(ccRBTree_t));
    set->compare_fn = compare_fn;
    set->head = NULL;
    set->keyed = keyed;

    return set;
}

void ccRBTree_dtor(ccRBTree_t *set)
{
    ccStack_t* stack = ccStack_ctor(512, NULL);
    ccRBTreeNode_t* node;

    ccStack_push(stack, set->head);
    while(stack->size != 0){
        node = ccStack_pop(stack);
        if(node->left != NULL){
            ccStack_push(stack, node->left);
        }
        if(node->right != NULL){
            ccStack_push(stack, node->right);
        }
        ccRBTreeNode_dtor(node);
    }

    ccStack_dtor(stack);
    free(set);
}

ccRBTreeNode_t* ccRBTreeNode_ctor(void* data, void* key, void (*dtor_data_fn)(void*), void (*dtor_key_fn)(void*))
{
    ccRBTreeNode_t* node = malloc(sizeof(ccRBTreeNode_t));
    node->color = red;
    node->item = data;
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    node->dtor_data_fn = dtor_data_fn;
    node->dtor_key_fn = dtor_key_fn;

    return node;
}

void ccRBTreeNode_dtor(ccRBTreeNode_t* node)
{
    if(node->dtor_data_fn != NULL)
        node->dtor_data_fn(node->item);
    if(node->dtor_key_fn != NULL)
        node->dtor_key_fn(node->key);
    free(node);
}

static void leftr(ccRBTree_t* set, ccRBTreeNode_t* node)
{
    ccRBTreeNode_t* other = node->right;
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

static void rightr(ccRBTree_t* set, ccRBTreeNode_t* node)
{
    ccRBTreeNode_t* other = node->left;
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

static void insert(ccRBTree_t* set, ccRBTreeNode_t* node, ccRBTreeNode_t* where)
{
    if(set->head == NULL){
        set->head = node;
        return;
    }
    if(where == NULL){
        where = set->head;
    }
    if(compareInsertImpl(set, node, where) > 0){
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

void insertRebalance(ccRBTree_t* set, ccRBTreeNode_t* node)
{
    ccRBTreeNode_t* other;
    /* color is already red from constructor */
    while(node != set->head && node->parent->color == red){
        if(node->parent->parent == NULL){
            node->color = black;
            return;
        }
        if(node->parent == node->parent->parent->left){
            other = node->parent->parent->right;
            if(other != NULL && other->color == red){
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
            if(other != NULL && other->color == red){
                node->parent->color = black;
                other->color = black;
                node->parent->parent->color = red;
                node = node->parent->parent;
            }else{
                if(node == node->parent->left){
                    node = node->parent;
                    rightr(set, node);
                }
                node->parent->color = black;
                node->parent->parent->color = red;
                leftr(set, node->parent->parent);
            }
        }
    }
}

void ccRBTree_insert(ccRBTree_t* set, ccRBTreeNode_t* node)
{
    insert(set, node, NULL);
    insertRebalance(set, node);
}

ccRBTreeNode_t* ccRBTree_find(ccRBTree_t* set, void* data)
{
    ccRBTreeNode_t* cursor = set->head;
    int result = 0;

    while(1){
        if(cursor == NULL)
            return NULL;
        result = compareImpl(set, cursor, data);
        if(result == 0){
            return cursor;
        }else if(result > 0){
                cursor = cursor->left;
        }else{
            cursor = cursor->right;
        }
    }
}

int compareImpl(ccRBTree_t* set, ccRBTreeNode_t* node, void* data)
{
    if(set->keyed){
        return set->compare_fn(node->key, data);
    }

    return set->compare_fn(node->item, data);
}

int compareInsertImpl(ccRBTree_t* set, ccRBTreeNode_t* node, ccRBTreeNode_t* where)
{
    if(set->keyed){
        return set->compare_fn(node->key, where->key);
    }

    return set->compare_fn(node->item, where->item);
}

static ccRBTreeNode_t* treeMin(ccRBTreeNode_t* tree)
{
    ccRBTreeNode_t* min = tree;
    while(min->left != NULL){
        min = min->left;
    }

    return min;
}

static void transplant(ccRBTree_t* set, ccRBTreeNode_t* a, ccRBTreeNode_t* b)
{
    if(a->parent == NULL){
        set->head = b;
    }else if(a == a->parent->left){
        a->parent->left = b;
    }else{
        a->parent->right = b;
    }
    if(b != NULL)
        b->parent = a->parent;
}

static void deleteRebalance(ccRBTree_t* set, ccRBTreeNode_t* node)
{
    ccRBTreeNode_t* other = NULL;

    while(node != set->head && node->color == black){
        if(node == node->parent->left){
            other = node->parent->right;
            if(other->color == red){
                other->color = black;
                node->parent->color = red;
                leftr(set, node->parent);
                other = node->parent->right;
            }
            if(other->left->color == black && other->right->color == black){
                other->color = red;
                node = node->parent;
            }else{
                if(other->right->color == black){
                    other->left->color = black;
                    other->color = red;
                    rightr(set, other);
                    other = node->parent->right;
                }
                other->color = node->parent->color;
                node->parent->color = black;
                other->right->color = black;
                leftr(set, node->parent);
                node = set->head;
            }
        }else{
            other = node->parent->left;
            if(other->color == red){
                other->color = black;
                node->parent->color = red;
                rightr(set, node->parent);
                other = node->parent->left;
            }
            if(other->right->color == black && other->left->color == black){
                other->color = red;
                node = node->parent;
            }else{
                if(other->left->color == black){
                    other->right->color = black;
                    other->color = red;
                    leftr(set, other);
                    other = node->parent->left;
                }
                other->color = node->parent->color;
                node->parent->color = black;
                other->left->color = black;
                rightr(set, node->parent);
                node = set->head;
            }
        }
    }

    node->color = black;
}

/* TODO: read about this more in detail */
void ccRBTree_remove(ccRBTree_t* set, void* data)
{
    ccRBTreeNode_t* node = ccRBTree_find(set, data);
    ccRBTreeNode_t* a = NULL;
    ccRBTreeNode_t* b = node;
    ccRBTreecolor_t originalColor;

    if(node == NULL)
        return;

    originalColor = node->color;

    if(node->left == NULL){
        a = node->right;
        transplant(set, node, node->right);
    }else if(node->right == NULL){
        a = node->left;
        transplant(set, node, node->left);
    }else{
        b = treeMin(node->right);
        originalColor = b->color;
        a = b;
        if(b->parent != node){
            transplant(set, b, b->right);
            b->right = node->right;
            b->right->parent = b;
        }
        transplant(set, node, b);
        b->left = node->left;
        b->left->parent = b;
        b->color = node->color;
    }
    if(originalColor == black){
        deleteRebalance(set, a);
    }

    ccRBTreeNode_dtor(node);
}

int ccRBTree_isEmpty(ccRBTree_t* set)
{
    if(set->head == NULL)
        return 1;
    return 0;
}

void dbg_printSet1(int* idx, ccRBTreeNode_t* node, void (*printData)(void*))
{
    if(node == NULL)
        return;

    if(node->left != NULL)
        dbg_printSet1(idx, node->left, printData);

    printData(node->item);

    if(node->right != NULL)
        dbg_printSet1(idx, node->right, printData);

    return;
}

void dbg_printSetKeyed1(int* idx, ccRBTreeNode_t* node, void (*printData)(void*))
{
    if(node == NULL)
        return;

    if(node->left != NULL)
        dbg_printSetKeyed1(idx, node->left, printData);

    printData(node);

    if(node->right != NULL)
        dbg_printSetKeyed1(idx, node->right, printData);

    return;
}


void dbg_printSet(ccRBTree_t* set, void (*printData)(void*))
{
    ccRBTreeNode_t* node = set->head;
    int idx = 0;
    int* idxp = &idx;

    if(set->keyed)
        dbg_printSetKeyed1(idxp, node, printData);
    else
        dbg_printSet1(idxp, node, printData);

    return;
}