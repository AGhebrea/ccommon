#pragma once

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "./cclog.h"

#ifndef DBGx
#define DBGx(expr) DBG(" = 0x%lx\n", (long unsigned int)expr)
#endif // DBGx

#ifndef DBGs
#define DBGs(expr) DBG(" = [%s]\n", (char*)expr)
#endif // DBGs

#ifndef DBG
#define DBG(fmt, expr) printf(#expr fmt, expr);
#endif // DBG

#ifndef ccLogDebug
#ifdef __GNUC__
#define ccLogDebug(str, ...) ccLog(ccLogLevels_Debug, __FILE__, __func__, str __VA_OPT__(,) __VA_ARGS__)
#else
#define ccLogDebug(str) ccLog(ccLogLevels_Debug, "", "", str)
#endif // __GNUC__
#endif // ccLogTrace

#ifndef ccLogTrace
#ifdef __GNUC__
#define ccLogTrace() ccLog(ccLogLevels_Trace, __FILE__, __func__, "")
#else
#define ccLogTrace() ccLog(ccLogLevels_Trace, "", "", "")
#endif // __GNUC__
#endif // ccLogTrace

#ifndef ccLogInfo
#ifdef __GNUC__
#define ccLogInfo(str, ...) ccLog(ccLogLevels_Info, __FILE__, __func__, str __VA_OPT__(,) __VA_ARGS__)
#else
#define ccLogInfo(str, ...) ccLog(ccLogLevels_Info, "", "", str)
#endif // __GNUC__
#endif // ccLogInfo

#ifndef ccLogWarn
#ifdef __GNUC__
#define ccLogWarn(str, ...) ccLog(ccLogLevels_Warn, __FILE__, __func__, str __VA_OPT__(,) __VA_ARGS__)
#else
#define ccLogWarn(str, ...) ccLog(ccLogLevels_Warn, "", "", str)
#endif // __GNUC__
#endif // ccLogInfo

#ifndef ccLogError
#ifdef __GNUC__
#define ccLogError(str, ...) ccLog(ccLogLevels_Error, __FILE__, __func__, str __VA_OPT__(,) __VA_ARGS__)
#else
#define ccLogError(str, ...) ccLog(ccLogLevels_Error, "", "", str)
#endif // __GNUC__
#endif // ccLogInfo

#ifndef expect
#define expect(arg, fcall, cond)                    \
({                                                  \
    int status = 1;                                 \
    arg = fcall;                                    \
    if(!(arg cond)){                                \
        int err = errno;                            \
        ccLogError(#fcall": %s", strerror(err));    \
        status = 0;                                 \
    }                                               \
    (status) ? 1 : 0;                               \
})
#endif // expect

#ifndef expectExit
#define expectExit(arg, fcall, cond)                \
do{                                                 \
    arg = fcall;                                    \
    if(!(arg cond)){                                \
        int err = errno;                            \
        ccLogError(#fcall": %s", strerror(err));    \
        exit(1);                                    \
    }                                               \
} while(0)
#endif // expectExit