#pragma once

#include "cl_common.h"
#include "stdio.h"

#ifdef __cplusplus
extern "C" {
#endif

//-------------event type------------------
typedef enum
{
    CL_Event_SgpRecvMsg = 0,
    CL_Event_TcpStaChanged,
    CL_Event_TcpDataFromServer,
    CL_Event_OneSecComplete,
    CL_Event_HasMan,
    CL_Event_SacPressure,
    CL_Event_OnNetworkCfg,
    CL_EventMax,
} CL_Event_t;

//---------------log-------------------------
#include "stdio.h"
#define USE_LDB_LOG
#define CL_PRINTF   printf

#ifdef __cplusplus
}
#endif
