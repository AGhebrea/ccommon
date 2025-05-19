#include <stdbool.h>

#include "./include/test_ccstd.h"
#include "./include/test_ccList.h"
#include "./include/test_ccStack.h"
#include "./include/test_ccSet.h"

int test_ccstd_main(void)
{
    int val = 0;

    val |= ccstd_cclist_smoketest();
    val |= ccstd_cclist_dtor_test();
    val |= ccstd_ccstack_smoketest();
    val |= ccstd_ccset_smoketest();

    return val;
}