#pragma once

#include "cl_common.h"

typedef CL_Result_t (*TestFunc)(void);

typedef struct 
{
    TestFunc func;
    char* desc;
} TestCase_t;


#define TEST_CASE_PROC(testCases, target_name) \
for (int i = 0; i < CL_ARRAY_LENGTH(testCases); i++) \
{   \
    if (testCases[i].func() != CL_ResSuccess)   \
    {   \
        printf("%s test failed at %s!!!!!!!!!!\n", target_name, testCases[i].desc);  \
        return 0;   \
    }   \
    printf("%s test %s ok...\n", target_name, testCases[i].desc); \
}   \
printf("%s all test ok...\n", target_name); 

// #define __DEBUG

#ifdef __DEBUG
#define DebugLog(format, ...)   printf(format, ##__VA_ARGS__)
#else
#define DebugLog(format, ...)
#endif
