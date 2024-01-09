#pragma once

#include "cl_common.h"
#include "clib_config.h"


#if defined(USE_LDB_LOG)

#define CL_LOG(format, ...) CL_PRINTF(format, ##__VA_ARGS__)
#define CL_LOG_LINE(format, ...)   \
    CL_PRINTF(format, ##__VA_ARGS__); \
    CL_PRINTF("\r\n")

#define CL_LOG_ERROR(format, ...)  \
    CL_PRINTF("!Error: "); \
    CL_PRINTF(format, ##__VA_ARGS__); \
    CL_PRINTF("\r\n")


#define CL_ASSERT(x) \
if(!(x)) \
{CL_LOG("assert failed at %s : %d", __FILE__, __LINE__); while(1);}



#else

#define CL_LOG(format, ...)
#define CL_LOG_LINE(format, ...) 
#define CL_LOG_ERROR(format, ...) 

#endif
