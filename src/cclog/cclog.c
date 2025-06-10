#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "./include/cclog.h"

ccLogLevel_t activeLogLevel = ccLogLevels_Debug;

#define PRINTBUFFERLEN 4096
char printBuffer[PRINTBUFFERLEN];

// todo: test vsnprintf fail path
void ccLog(ccLogLevel_t level, const char* file, const char* function, const char *fmt, ...)
{
    size_t size;
    char *p;
    va_list ap;
    int n = 0;
    bool free_mem = false;

    if(!ccLog_isLogLevelActive(level))
        return;

    size = 0;
    p = NULL;

    va_start(ap, fmt);
    if(!expect(n, vsnprintf(p, size, fmt, ap), >= 0))
        return;
    va_end(ap);

    size = (size_t) n + 1;
    if(size > PRINTBUFFERLEN){
        if(!expect(p, malloc(size), != NULL))
            return;
        free_mem = true;
    }else{
        p = printBuffer;
    }

    va_start(ap, fmt);
    n = vsnprintf(p, size, fmt, ap);
    va_end(ap);

    if(function[0] != '\0'){
        fprintf(stdout, "[%s %s::%s()] %s\n", logLevelString[level], file, function, p);
    }
    else{
        fprintf(stdout, "[%s] %s\n", logLevelString[level], p);
    }
   
    if (free_mem) {
        free(p);
    }

    return;
}

void ccLog_setLogLevel(ccLogLevel_t level)
{
    if(level > ccLogLevels_Off || level < 0)
        level = ccLogLevels_Off;
    activeLogLevel = level;
}

ccLogLevel_t ccLog_getActiveLogLevel()
{
    return activeLogLevel;
}

int ccLog_isLogLevelActive(ccLogLevel_t level)
{
    if(level < activeLogLevel)
        return 0;
    return 1;
}