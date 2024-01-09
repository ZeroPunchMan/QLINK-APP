/**********************************************
   
    CL_FsmState fsmStates[] =
    {
        CL_FSM_STATE(PowerUpStart, PowerUpStop, PowerUpUpdate),      //power up
        CL_FSM_STATE(CL_NULL, CL_NULL, WorkingUpdate),      //working
        CL_FSM_STATE(RebootStart, RebootStop, RebootUpdate),      //reboot
    };

    CL_FSM_DEF(testFsm, fsmStates, PowerUp);

    CL_FsmUpdate(&testFsm, interval);
 **********************************************/
#pragma once

#include "cl_common.h"

#ifdef __cplusplus
extern "C" {
#endif

struct CL_Fsm;
//---------------------------------
typedef void(*CL_StateStart)(struct CL_Fsm* fsm, void* startParam);
typedef void(*CL_StateStop)(struct CL_Fsm* fsm);
typedef void(*CL_StateUpdate)(struct CL_Fsm* fsm); 

typedef struct CL_FsmState
{
    CL_StateStart onStart;	
    CL_StateStop onStop;	
    CL_StateUpdate update;	//update
} CL_FsmState_t; 

typedef struct CL_Fsm
{
    CL_FsmState_t* states;
    uint8_t statesNum;	
    uint8_t curStateIdx; 
} CL_Fsm_t;      

extern void CL_FsmUpdate(CL_Fsm_t* fsm, uint16_t interval);
extern void CL_FsmChangeState(CL_Fsm_t* fsm, uint8_t stateIndex, void* startParam);
extern uint8_t CL_FsmGetState(CL_Fsm_t* fsm);
extern void CL_FsmInit(CL_Fsm_t* fsm, void* startParam);



#define CL_FSM_STATE(start, stop, update) \
    {start, stop, update}

#define CL_FSM_DEF(name, state_array, firstState) \
CL_Fsm_t name = {state_array, CL_ARRAY_LENGTH(state_array), firstState};

#define CL_FSM_DECL(name) \
extern CL_Fsm_t name;

#ifdef __cplusplus
}
#endif
