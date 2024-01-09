#include "cl_fsm.h"

void CL_FsmInit(CL_Fsm_t* fsm, void* startParam)
{
    CL_StateStart startFunc;
    startFunc = fsm->states[fsm->curStateIdx].onStart;
    if (startFunc != CL_NULL)
        startFunc(fsm, startParam);
} 

void CL_FsmUpdate(CL_Fsm_t* fsm, uint16_t interval)
{
    CL_StateUpdate updateFunc;

    updateFunc = fsm->states[fsm->curStateIdx].update;
    if (updateFunc != CL_NULL)
        updateFunc(fsm);
}

void CL_FsmChangeState(CL_Fsm_t* fsm, uint8_t stateIndex, void* startParam)
{
    CL_StateStart startFunc;
    CL_StateStop stopFunc;
//    assert(fsm->curStateIdx < fsm->statesNum);

    if (stateIndex < fsm->statesNum)
    {
        //ֹͣ��ǰ״̬
        stopFunc = fsm->states[fsm->curStateIdx].onStop;
        if (stopFunc != CL_NULL)
            stopFunc(fsm);

        //��ʼ��״̬
        fsm->curStateIdx = stateIndex;
        startFunc = fsm->states[fsm->curStateIdx].onStart;
        if (startFunc != CL_NULL)
            startFunc(fsm, startParam);
    }
}

uint8_t CL_FsmGetState(CL_Fsm_t* fsm)
{
    return fsm->curStateIdx;
}

