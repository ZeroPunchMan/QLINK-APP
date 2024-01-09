#include "cl_event_system.h"
#include "cl_list.h"
#include "cl_pool.h"

typedef struct
{
    CL_ListNode_t node;
    CL_EventCallBack_t callBack;
    int session;
} Listener_t;

CL_POOL_DEFINE(lsrPool, CL_EventMax * 5, Listener_t, static);
static CL_List_t lsrListArray[CL_EventMax];
static bool initialized = false;

void CL_EventSysInit(void)
{
    CL_PoolInit(&lsrPool);
    for (int i = 0; i < CL_EventMax; i++)
    {
        CL_ListInit(&lsrListArray[i]);
    }
    initialized = true;
}

CL_Result_t CL_EventSysAddListener(CL_EventCallBack_t cb, CL_Event_t event, int session)
{
    if(!initialized)
        return CL_ResNotInit;

    if (event >= CL_EventMax)
    {
        return CL_ResInvalidParam;
    }

    Listener_t *pLsr = CL_POOL_ALLOC(&lsrPool, Listener_t);
    if (pLsr == CL_NULL)
    {
        return CL_ResFailed;
    }

    pLsr->callBack = cb;
    pLsr->session = session;

    return CL_ListAddLast(&lsrListArray[event], &pLsr->node);
}

CL_Result_t CL_EventSysRemoveListener(CL_EventCallBack_t cb, CL_Event_t event, int session)
{
    if(!initialized)
        return CL_ResNotInit;

    if (event >= CL_EventMax)
    {
        return CL_ResInvalidParam;
    }

    CL_List_t *lsrList = &lsrListArray[event];
    Listener_t *pLsr;
    CL_LIST_FOR_EACH_ENTRY_SAFE(lsrList, pLsr, Listener_t, node)
    {
        if (pLsr->callBack == cb && pLsr->session == session)
        {
            CL_Result_t res;
            res = CL_ListRemove(lsrList, &pLsr->node);
            if (res != CL_ResSuccess)
            {
                return res;
            }
            res = CL_POOL_FREE(&lsrPool, pLsr);
            if (res != CL_ResSuccess)
            {
                return res;
            }
            return CL_ResSuccess;
        }
    }

    return CL_ResFailed;
}

CL_Result_t CL_EventSysRaise(CL_Event_t event, int session, void *eventArg)
{
    if(!initialized)
        return CL_ResNotInit;

    if (event >= CL_EventMax)
    {
        return CL_ResInvalidParam;
    }

    CL_List_t *lsrList = &lsrListArray[event];
    Listener_t *pLsr;
    CL_LIST_FOR_EACH_ENTRY_SAFE(lsrList, pLsr, Listener_t, node)
    {
        if (pLsr->session == session)
        {
            pLsr->callBack(eventArg);
        }
    }

    return CL_ResSuccess;
}

CL_Result_t CL_EventCleanCheck(void)
{
    for (int i = 0; i < CL_EventMax; i++)
    {
        CL_List_t *lsrList = &lsrListArray[i];
        Listener_t *pLsr;

        // printf("check event %d\n", i);
        CL_LIST_FOR_EACH_ENTRY(lsrList, pLsr, Listener_t, node)
        {
            return CL_ResFailed;
        }
    }

    return CL_PoolFullCheck(&lsrPool);
}
