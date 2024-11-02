#pragma once
#include "cl_common.h"

void Comm_Init(void);
void Comm_Process(void);


void Comm_SendError(uint8_t err);
void Comm_SendMode(uint8_t m);
void Comm_SendRunPause(bool run);
