#include <stdbool.h>

#include "./include/test_ccstd.h"
#include "../src/cclog/include/cclog_macros.h"
#include "../src/ccstd/include/ccList.h"

void printList(ccList_t* list, char* str)
{
    return;

    ccListNode_t* tmp;

    printf("\n\n[%s]\n", str);

    tmp = list->head;
    while(tmp != NULL){
        DBGx(tmp->previous);
        DBGx(tmp);
        DBGx(tmp->data);
        DBGx(*(size_t*)tmp->data);
        DBGx(tmp->next);
        printf("\n\n");

        tmp = tmp->next;
    }
}


int ccstd_cclist_smoketest(void)
{
    ccListNode_t* tmp;
    ccList_t* newlist;
    size_t data_arr[4] = {0xdeadbeef, 0xdeadbeef, 0xdeadb00f, 0xdeadb00b};
    size_t i = 0;
    bool failed = false;

    newlist = ctor_ccList();
    insert_ccList(newlist, 0, ctor_ccListNode(&data_arr[0], NULL));
    printList(newlist, "1");
    
    insert_ccList(newlist, 0, ctor_ccListNode(&data_arr[1], NULL));
    printList(newlist, "1");

    insert_ccList(newlist, 1, ctor_ccListNode(&data_arr[3], NULL));
    printList(newlist, "1");

    insert_ccList(newlist, 1, ctor_ccListNode(&data_arr[2], NULL));
    printList(newlist, "1");

    insert_ccList(newlist, 1, ctor_ccListNode(&data_arr[2], NULL));
    printList(newlist, "1");

    insert_ccList(newlist, 9, ctor_ccListNode(&data_arr[2], NULL));
    printList(newlist, "1");

    delete_item_ccList(newlist, 10);
    printList(newlist, "1");

    delete_item_ccList(newlist, 2);
    printList(newlist, "1");

    tmp = newlist->head;
    while(tmp != NULL)
    {
        if(tmp->data == NULL){
            failed = true;
            ccLogError("item at 0x%x has NULL data!");
        }else if(*(size_t*)tmp->data != data_arr[i]){
            failed = true;
            ccLogError("item at 0x%x has incorrect data! (0x%x) but should be (0x%x)", tmp, *(size_t*)tmp->data, data_arr[i]);
        }

        if(i != 3 && tmp->next == NULL){
            failed = true;
            ccLogError("item at 0x%x (index = %ld) has incorrect next pointer!", tmp, i);
        }

        if(i != 0 && tmp->previous == NULL){
            failed = true;
            ccLogError("item at 0x%x (index = %ld) has incorrect previous pointer!", tmp, i);
        }

        tmp = tmp->next;
        ++i;
    }

    dtor_ccList(newlist);

    return failed;
}

typedef struct test_data{
    char* mallocd_string;
    int* mallocd_array;
}test_data_t;

test_data_t* ctor_test_data(void)
{
    test_data_t* p;

    p = malloc(sizeof(test_data_t));
    p->mallocd_string = malloc(sizeof(char) * 100);
    p->mallocd_array = malloc(sizeof(int) * 100);

    return p;
}

void dtor_test_data(void* p)
{
    test_data_t* cast;

    cast = (test_data_t*)p;

    if(cast->mallocd_string != NULL)
        free(cast->mallocd_string);
    if(cast->mallocd_array != NULL)
        free(cast->mallocd_array);
    free(cast);

    return;
}

int ccstd_cclist_dtor_test(void)
{
    ccList_t* newlist;
    bool failed = false;

    newlist = ctor_ccList();
    insert_ccList(newlist, 0, ctor_ccListNode((void*)ctor_test_data(), &dtor_test_data));
    printList(newlist, "1");
    
    insert_ccList(newlist, 0, ctor_ccListNode((void*)ctor_test_data(), &dtor_test_data));
    printList(newlist, "1");

    insert_ccList(newlist, 1, ctor_ccListNode((void*)ctor_test_data(), &dtor_test_data));
    printList(newlist, "1");

    insert_ccList(newlist, 1, ctor_ccListNode((void*)ctor_test_data(), &dtor_test_data));
    printList(newlist, "1");

    insert_ccList(newlist, 1, ctor_ccListNode((void*)ctor_test_data(), &dtor_test_data));
    printList(newlist, "1");

    insert_ccList(newlist, 9, ctor_ccListNode((void*)ctor_test_data(), &dtor_test_data));
    printList(newlist, "1");

    delete_item_ccList(newlist, 10);
    printList(newlist, "1");

    delete_item_ccList(newlist, 2);
    printList(newlist, "1");

    dtor_ccList(newlist);

    return failed;
}