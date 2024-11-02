#pragma once

#include "cl_common.h"

#define BEEP_DURATION   (150)
#define BEEP_INTERVAL   (80)

void Buzzer_Init(void);
void Buzzer_Process(void);

void Buzzer_SetBeep(uint8_t count);
void Buzzer_Stop(void);
