#include "../src/cclog/include/cclog.h"
#include "./include/test_ccstd.h"
#include "./include/test_ccRBTree.h"

int main()
{
    int status = 0;

    ccLog_setLogLevel(ccLogLevels_Error);
    // ccLog_setLogLevel(ccLogLevels_Trace);
    // ccLog_setLogLevel(ccLogLevels_Off);

    status |= test_ccstd_main();
    // todo: mem leak tests

    return status;
}