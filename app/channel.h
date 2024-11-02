#pragma once

#include "cl_common.h"

typedef enum
{
    ChanIdx_1 = 0,
    ChanIdx_2,
    ChanIdx_3,
    ChanIdx_Max,
} ChanIdx_t;

#define LOW_FREQ_PERIOD (1100)
#define HIGH_FREQ_PERIOD (550)

void Channel_Init(void);
void Channel_Process(void);

void Channel_SetEnergy(ChanIdx_t chan, uint8_t energy);
