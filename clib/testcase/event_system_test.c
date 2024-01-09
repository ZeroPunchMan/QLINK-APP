#include "test_common.h"
#include "cl_event_system.h"
#include "stdlib.h"
#include "time.h"

#define TEST_CB_DEF(event, session)                            \
    static int CbCalled_##event##_##session = 0;               \
    static bool EventCb_##event##_##session(void *eventArg) \
    {                                                          \
        CbCalled_##event##_##session++;                        \
        int32_t *pInt = (int32_t *)eventArg;                   \
        DebugLog("event call back %d--%d\n", event, session);    \
        if (pInt != CL_NULL)                                   \
            *pInt = event * session;                           \
    }

#define TEST_CB_REF(event, session) (EventCb_##event##_##session)
#define TEST_FLAG_REF(event, session) (CbCalled_##event##_##session)
#define TEST_FLAG_CLEAR(event, session) (CbCalled_##event##_##session = 0)

#define TEST_ADD_LISTENER(event, session) CL_EventSysAddListener(TEST_CB_REF(event, session), event, session)
#define TEST_RM_LISTENER(event, session) CL_EventSysRemoveListener(TEST_CB_REF(event, session), event, session)

TEST_CB_DEF(0, 0);
TEST_CB_DEF(0, 1);
TEST_CB_DEF(0, 2);
TEST_CB_DEF(0, 3);
TEST_CB_DEF(0, 4);
TEST_CB_DEF(1, 0);
TEST_CB_DEF(1, 1);
TEST_CB_DEF(1, 2);
TEST_CB_DEF(1, 3);
TEST_CB_DEF(1, 4);
TEST_CB_DEF(2, 0);
TEST_CB_DEF(2, 1);
TEST_CB_DEF(2, 2);
TEST_CB_DEF(2, 3);
TEST_CB_DEF(2, 4);
TEST_CB_DEF(3, 0);
TEST_CB_DEF(3, 1);
TEST_CB_DEF(3, 2);
TEST_CB_DEF(3, 3);
TEST_CB_DEF(3, 4);
TEST_CB_DEF(4, 0);
TEST_CB_DEF(4, 1);
TEST_CB_DEF(4, 2);
TEST_CB_DEF(4, 3);
TEST_CB_DEF(4, 4);

typedef struct
{
    int32_t *pCbFlag;
    CL_EventCallBack_t callback;
} TestStruct_t;

#define TEST_STRUCT_REF(event, session)                                                    \
    {                                                                                      \
        .pCbFlag = &TEST_FLAG_REF(event, session), .callback = TEST_CB_REF(event, session) \
    }

#define TEST_SIZE 5

TestStruct_t testStructs[TEST_SIZE][TEST_SIZE] = {
    {
        TEST_STRUCT_REF(0, 0),
        TEST_STRUCT_REF(0, 1),
        TEST_STRUCT_REF(0, 2),
        TEST_STRUCT_REF(0, 3),
        TEST_STRUCT_REF(0, 4),
    },
    {
        TEST_STRUCT_REF(1, 0),
        TEST_STRUCT_REF(1, 1),
        TEST_STRUCT_REF(1, 2),
        TEST_STRUCT_REF(1, 3),
        TEST_STRUCT_REF(1, 4),
    },
    {
        TEST_STRUCT_REF(2, 0),
        TEST_STRUCT_REF(2, 1),
        TEST_STRUCT_REF(2, 2),
        TEST_STRUCT_REF(2, 3),
        TEST_STRUCT_REF(2, 4),
    },
    {
        TEST_STRUCT_REF(3, 0),
        TEST_STRUCT_REF(3, 1),
        TEST_STRUCT_REF(3, 2),
        TEST_STRUCT_REF(3, 3),
        TEST_STRUCT_REF(3, 4),
    },
    {
        TEST_STRUCT_REF(4, 0),
        TEST_STRUCT_REF(4, 1),
        TEST_STRUCT_REF(4, 2),
        TEST_STRUCT_REF(4, 3),
        TEST_STRUCT_REF(4, 4),
    }};

bool cbLsnFlags[TEST_SIZE][TEST_SIZE];

CL_Result_t EventSysTest(void)
{
    const int testTimes = 500000;
    int32_t eventArg;
    CL_EventSysInit();

    CL_Result_t res;
    res = CL_EventCleanCheck();
    if(res != CL_ResSuccess)
    {
        return res;
    }

    for (int i = 0; i < TEST_SIZE; i++)
    {
        for (int k = 0; k < TEST_SIZE; k++)
        {
            if (CL_EventSysAddListener(testStructs[i][k].callback, i, k) != CL_ResSuccess)
            {
                DebugLog("add failed at %d--%d\n", i, k);
                return CL_ResFailed;
            }
            cbLsnFlags[i][k] = true;
        }
    }


    srand(time(NULL));
    for (int i = 0; i < testTimes; i++)
    {
        DebugLog("------------test %d-------------\n", i);
        eventArg = 0;

        for (int k = 0; k < TEST_SIZE; k++)
        {
            for (int m = 0; m < TEST_SIZE; m++)
            {
                *testStructs[k][m].pCbFlag = 0;
            }
        }

        int e, s;
        e = rand() % TEST_SIZE;
        s = rand() % TEST_SIZE;

        int r = rand() % 3;
        if (r == 0)
        { //add
            DebugLog("add: %d, %d\n", e, s);
            if (cbLsnFlags[e][s] == false)
            {
                CL_EventSysAddListener(testStructs[e][s].callback, e, s);
                cbLsnFlags[e][s] = true;
            }
        }
        else if (r == 1)
        { //remove
            DebugLog("remove: %d, %d\n", e, s);
            if (cbLsnFlags[e][s] == true)
            {
                CL_EventSysRemoveListener(testStructs[e][s].callback, e, s);
                cbLsnFlags[e][s] = false;
            }
        }
        else
        { //keep
            DebugLog("keep\n");
        }

        DebugLog("/////////////////////////\n");
        for (int k = 0; k < TEST_SIZE; k++)
        {
            for (int m = 0; m < TEST_SIZE; m++)
            {
                DebugLog("[%d, %d]: %d\t", k, m, cbLsnFlags[k][m]);
            }
            DebugLog("\n");
        }
        DebugLog("/////////////////////////\n");

        e = rand() % TEST_SIZE;
        s = rand() % TEST_SIZE;
        int raiseTimes = (rand() % 30) + 1;
        DebugLog("raise: %d, %d; times: %d\n", e, s, raiseTimes);
        for (int k = 0; k < raiseTimes; k++)
        {
            CL_EventSysRaise(e, s, &eventArg);
        }

        DebugLog("event arg: %d\n", eventArg);
        if (cbLsnFlags[e][s] == true)
        {
            if (eventArg != e * s)
            {
                DebugLog("event arg error\n");
                return CL_ResFailed;
            }
        }
        else
        {
            if (eventArg != 0)
            {
                DebugLog("event arg error2\n");
                return CL_ResFailed;
            }
        }

        if (cbLsnFlags[e][s] == true)
        { //listened the event
            DebugLog("**********************\n");
            for (int k = 0; k < TEST_SIZE; k++)
            {
                for (int m = 0; m < TEST_SIZE; m++)
                {
                    DebugLog("[%d, %d]: %d\t", k, m, *testStructs[k][m].pCbFlag);
                    if (k == e && m == s)
                    {
                        continue;
                    }
                    else
                    {
                        if (*testStructs[k][m].pCbFlag != 0)
                        {
                            return CL_ResFailed;
                        }
                    }
                }
                DebugLog("\n");
            }

            if (*testStructs[e][s].pCbFlag != raiseTimes)
            {
                return CL_ResFailed;
            }
        }
        else
        { //did not listen the event
            DebugLog("----------------------\n");
            for (int k = 0; k < TEST_SIZE; k++)
            {
                for (int m = 0; m < TEST_SIZE; m++)
                {
                    DebugLog("[%d, %d]: %d\t", k, m, *testStructs[k][m].pCbFlag);
                    if (*testStructs[k][m].pCbFlag != 0)
                    {
                        return CL_ResFailed;
                    }
                }
                DebugLog("\n");
            }
        }
    }

    //remove all listener
    for (int i = 0; i < TEST_SIZE; i++)
    {
        for (int k = 0; k < TEST_SIZE; k++)
        {
            if(cbLsnFlags[i][k] != false)
            {
                res = CL_EventSysRemoveListener(testStructs[i][k].callback, i, k);
                if(res != CL_ResSuccess)
                {
                    return res;
                }
                cbLsnFlags[i][k] = false;
            }
        }
    }

    return CL_EventCleanCheck();
}

static CL_Result_t InitTest(void)
{
    if(CL_EventSysAddListener(TEST_CB_REF(0, 0), 0, 0) != CL_ResNotInit)
    {
        return CL_ResFailed;
    }

    if(CL_EventSysRemoveListener(TEST_CB_REF(0, 0), 0, 0) != CL_ResNotInit)
    {
        return CL_ResFailed;
    }

    if(CL_EventSysRaise(0, 0, CL_NULL) != CL_ResNotInit)
    {
        return CL_ResFailed;
    }

    return CL_ResSuccess;
}

TestCase_t testCases[] = {
    {InitTest, "init"},
    {EventSysTest, "wtf"},
};

int main(int argc, char **argv)
{
    TEST_CASE_PROC(testCases, "event system");
}
