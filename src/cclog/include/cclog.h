#pragma once

#include "cclog_macros.h"

typedef enum ccLogLevel{
    ccLogLevels_Trace,
    ccLogLevels_Debug,
    ccLogLevels_Info,
    ccLogLevels_Error,
    ccLogLevels_Off
}ccLogLevel_t;

// weak attrib to avoid ODR violation
#ifdef __GNUC__
char const* logLevelString[ccLogLevels_Off] __attribute__((weak)) = {
    "Trace",
    "Debug",
    "Info",
    "Error"
};
#else
// idk man, maybe just let the build fail because logLevelString does not exist?
#endif // __GNUC__

void setLogLevel(ccLogLevel_t);
void ccLog(ccLogLevel_t level, const char* file, const char* function, const char *fmt, ...);