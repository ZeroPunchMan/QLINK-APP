#include "cl_common.h"

//*********************queue******************************

typedef enum
{
    QMStyle_AddCell = 0,
    QMStyle_AddBulk,
    QMStyle_AddBulkOrLess,
    QMStyle_AddMax,
} QueueMtestAddStyle_t;

typedef enum
{
    QMStyle_PollCell = 0,
    QMStyle_PollBulk,
    QMStyle_PollBulkOrLess,
    QMStyle_PollMax,
} QueueMtestPollStyle_t;

void CL_MtestQueueInit(uint32_t buffSize,
                       uint32_t marginSize,
                       uint16_t qCellSize,
                       uint16_t lenOfBulk,
                       QueueMtestAddStyle_t addStyle,
                       QueueMtestPollStyle_t pollStyle);
void CL_MtestQueueExit(void);

void CL_MTestQueueStart(void);
void CL_MTestQueueStop(void);

bool CL_MTestQueueAdd(uint16_t delay, bool checkMem);
bool CL_MTestQueuePoll(uint16_t delay, bool checkMem);
uint32_t CL_MTestQueueGetPollCount(void);
//----------------------------------------------------
