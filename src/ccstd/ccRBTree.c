#include <stdlib.h>
#include "./include/ccRBTree.h"
#include "include/ccStack.h"

int compareImpl(ccRBTree_t* set, ccRBTreeNode_t* node, void* data);
int compareInsertImpl(ccRBTree_t* set, ccRBTreeNode_t* node, ccRBTreeNode_t* where);
void insertRebalance(ccRBTree_t* set, ccRBTreeNode_t* node);

ccRBTree_t* ccRBTree_ctor(int (*compare_fn)(void*, void*))
{
    ccRBTree_t* set = malloc(sizeof(ccRBTree_t));
    set->null = ccRBTreeNode_ctor(NULL, NULL, NULL, NULL);
    set->null->color = black;
    set->compare_fn = compare_fn;
    set->head = set->null;
    set->size = 0;

    ccType_ctor(&set->type, "ccRBTree_t");

    return set;
}

void ccRBTree_dtor(ccRBTree_t *set)
{
    ccRBTreeNode_t* node;

    if(set == NULL)
        return;

    ccStack_t* stack = ccStack_ctor(512, NULL);

    if(!ccRBTree_isLeaf(set, set->head))
        ccStack_push(stack, set->head);
    while(stack->size != 0){
        node = ccStack_pop(stack);
        if(!ccRBTree_isLeaf(set, node->left)){
            ccStack_push(stack, node->left);
        }
        if(!ccRBTree_isLeaf(set, node->right)){
            ccStack_push(stack, node->right);
        }
        ccRBTreeNode_dtor(node);
    }

    ccStack_dtor(stack);
    ccRBTreeNode_dtor(set->null);
    free(set);
}

ccRBTreeNode_t* ccRBTreeNode_ctor(void* data, void* key, void (*dtor_data_fn)(void*), void (*dtor_key_fn)(void*))
{
    ccRBTreeNode_t* node = malloc(sizeof(ccRBTreeNode_t));
    node->color = red;
    node->key = key;
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    node->dtor_data_fn = dtor_data_fn;
    node->dtor_key_fn = dtor_key_fn;

    ccType_ctor(&node->type, "ccRBTreeNode_t");

    return node;
}

void ccRBTreeNode_dtor(ccRBTreeNode_t* node)
{
    if(node == NULL)
        return;
    if(node->dtor_data_fn != NULL)
        node->dtor_data_fn(node->data);
    if(node->dtor_key_fn != NULL)
        node->dtor_key_fn(node->key);
    free(node);
}

static void leftr(ccRBTree_t* set, ccRBTreeNode_t* node)
{
    ccRBTreeNode_t* other = node->right;

    node->right = other->left;
    if(other->left != set->null)
        other->left->parent = node;
    other->parent = node->parent;
    if(node->parent == set->null){
        set->head = other;
    }else if(node == node->parent->left){
        node->parent->left = other;
    }else{
        node->parent->right = other;
    }
    other->left = node;
    node->parent = other;
}

static void rightr(ccRBTree_t* set, ccRBTreeNode_t* node)
{
    ccRBTreeNode_t* other = node->left;

    node->left = other->right;
    if(other->right != set->null)
        other->right->parent = node;
    other->parent = node->parent;
    if(node->parent == set->null){
        set->head = other;
    }else if(node == node->parent->right){
        node->parent->right = other;
    }else{
        node->parent->left = other;
    }
    other->right = node;
    node->parent = other;
}

void ccRBTree_insert(ccRBTree_t* set, ccRBTreeNode_t* node)
{
    ccRBTreeNode_t* other;
    ccRBTreeNode_t* where;

    set->size += 1;

    where = set->null;
    other = set->head;
    while(other != set->null){
        where = other;
        if(set->compare_fn(node->key, other->key) >= 0){
            other = other->right;
        }else{
            other = other->left;
        }
    }
    node->parent = where;
    if(where == set->null){
        set->head = node;
    }else if(set->compare_fn(node->key, where->key) >= 0){
        where->right = node;
    }else{
        where->left = node;
    }
    node->left = set->null;
    node->right = set->null;
    node->color = red;
    insertRebalance(set, node);
}

void insertRebalance(ccRBTree_t* set, ccRBTreeNode_t* node)
{
    ccRBTreeNode_t* other;
    
    while(node->parent->color == red){
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
                    rightr(set, node);
                }
                node->parent->color = black;
                node->parent->parent->color = red;
                leftr(set, node->parent->parent);
            }
        }
    }
    set->head->color = black;
}

int ccRBTree_contains(ccRBTree_t* set, void* key)
{
    if(ccRBTree_find(set, key) != NULL)
        return 1;
    return 0;
}

ccRBTreeNode_t* ccRBTree_find(ccRBTree_t* set, void* key)
{
    ccRBTreeNode_t* cursor = set->head;
    int result = 0;

    while(1){
        if(cursor == set->null)
            return NULL;
        result = set->compare_fn(key, cursor->key);
        if(result == 0){
            return cursor;
        }else if(result > 0){
            cursor = cursor->right;
        }else{
            cursor = cursor->left;
        }
    }
}

static ccRBTreeNode_t* treeMin(ccRBTree_t* set, ccRBTreeNode_t* tree)
{
    ccRBTreeNode_t* min = tree;
    if(tree == set->null)
        return NULL;
    while(min->left != set->null){
        min = min->left;
    }

    return min;
}

static void transplant(ccRBTree_t* set, ccRBTreeNode_t* a, ccRBTreeNode_t* b)
{
    if(a->parent == set->null){
        set->head = b;
    }else if(a == a->parent->left){
        a->parent->left = b;
    }else{
        a->parent->right = b;
    }
    b->parent = a->parent;
}

static void deleteRebalance(ccRBTree_t* set, ccRBTreeNode_t* node)
{
    ccRBTreeNode_t* other;

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

int ccRBTree_isLeaf(ccRBTree_t* set, ccRBTreeNode_t* node)
{
    if(node == set->null)
        return 1;
    return 0;
}

void ccRBTree_removeNode(ccRBTree_t* set, ccRBTreeNode_t* node)
{
    ccRBTreeNode_t* a;
    ccRBTreeNode_t* b;
    ccRBTreecolor_t original;

    if(node == NULL)
        return;

    b = node;
    original = b->color;
    if(node->left == set->null){
        a = node->right;
        transplant(set, node, node->right);
    }else if(node->right == set->null){
        a = node->left;
        transplant(set, node, node->left);
    }else{
        b = treeMin(set, node->right);
        original = b->color;
        a = b->right;
        if(b->parent == node){
            a->parent = b;
        }else{
            transplant(set, b, b->right);
            b->right = node->right;
            b->right->parent = b;
        }
        transplant(set, node, b);
        b->left = node->left;
        b->left->parent = b;
        b->color = node->color;
    }

    if(original == black)
        deleteRebalance(set, a);
}

void ccRBTree_remove(ccRBTree_t* set, void* key)
{
    ccRBTreeNode_t* node = ccRBTree_find(set, key);
    ccRBTree_removeNode(set, node);
}

int ccRBTree_isEmpty(ccRBTree_t* set)
{
    if(set->head == set->null)
        return 1;
    return 0;
}

void dbg_printSet1(ccRBTree_t* set, ccRBTreeNode_t* node, void (*printData)(void*))
{
    if(node == set->null)
        return;

    if(node->left != set->null)
        dbg_printSet1(set, node->left, printData);

    printData(node);

    if(node->right != set->null)
        dbg_printSet1(set, node->right, printData);

    return;
}


void dbg_printSet(ccRBTree_t* set, void (*printData)(void*))
{
    ccRBTreeNode_t* node = set->head;

    dbg_printSet1(set, node, printData);

    return;
}