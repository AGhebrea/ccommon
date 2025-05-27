#include <stdbool.h>

#include "./include/test_ccstd.h"
#include "./include/test_ccList.h"
#include "./include/test_ccStack.h"
#include "./include/test_ccRBTree.h"
#include "./include/test_ccDSU.h"

int test_ccstd_main(void)
{
    int val = 0;

    val |= ccstd_cclist_smoketest();
    val |= ccstd_cclist_jointest();
    val |= ccstd_cclist_dtor_test();
    val |= ccstd_ccstack_smoketest();
    val |= ccstd_ccRBTree_smoketest();
    val |= ccstd_ccDSU_smoketest();

    return val;
}