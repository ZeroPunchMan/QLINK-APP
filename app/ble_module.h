#pragma once
#include "cl_common.h"

typedef enum
{
    Ble_NoConnect,
    Ble_Connected,
} BleModuleWorkMode_t;

void BleModule_Init(void);
void BleModule_Process(void);

BleModuleWorkMode_t BleModule_GetWorkMode(void);

