#pragma once

#include "cl_common.h"
#include "clib_config.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef bool (*CL_EventCallBack_t)(void* eventArg);

void CL_EventSysInit(void);

CL_Result_t CL_EventSysAddListener(CL_EventCallBack_t cb, CL_Event_t event, int session);

CL_Result_t CL_EventSysRemoveListener(CL_EventCallBack_t cb, CL_Event_t event, int session);

CL_Result_t CL_EventSysRaise(CL_Event_t event, int session, void *eventArg);

CL_Result_t CL_EventCleanCheck(void);


#ifdef __cplusplus
}
#endif
