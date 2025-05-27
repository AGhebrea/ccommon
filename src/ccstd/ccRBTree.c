#include <stdlib.h>
#include "./include/ccRBTree.h"
#include "../cclog/include/cclog_macros.h"

ccRBTree_t* ccRBTree_ctor(void (*dtor_data_fn)(void*), int (*compare_fn)(void*, void*))
{
    ccRBTree_t* set = malloc(sizeof(ccRBTree_t));
    set->dtor_data_fn = dtor_data_fn;
    set->compare_fn = compare_fn;
    set->head = NULL;

    return set;
}

void ccRBTree_dtor(ccRBTree_t *set)
{
    ccLogNotImplemented;
}

ccRBTreeNode_t* ccRBTreeNode_ctor(void* data)
{
    ccRBTreeNode_t* node = malloc(sizeof(ccRBTreeNode_t));
    node->color = red;
    node->item = data;
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;

    return node;
}

void ccRBTreeNode_dtor(ccRBTreeNode_t* node)
{
    ccLogNotImplemented;
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
}

void ccRBTree_insert(ccRBTree_t* set, ccRBTreeNode_t* node)
{
    insert(set, node, NULL);
    insertRebalance(set, node);
}

ccRBTreeNode_t* ccRBTree_find(ccRBTree_t* set, void* data)
{
    ccRBTreeNode_t* cursor = set->head;

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
    ccRBTreecolor_t originalColor = node->color;

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

struct cust{
    int i;
    char c;
};

void dbg_printSet1(struct cust* data, int* idx, ccRBTreeNode_t* node)
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

void dbg_printSet(ccRBTree_t* set)
{
    ccRBTreeNode_t* node = set->head;
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