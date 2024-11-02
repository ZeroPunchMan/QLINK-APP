#include "ble_module.h"
#include "board.h"
#include "cl_log.h"
#include "systime.h"
#include "led.h"

void BleModule_Init(void)
{
}

static BleModuleWorkMode_t moduleWorkMode = Ble_NoConnect;
static inline bool GetBleStaPin(void)
{
    return LL_GPIO_IsInputPinSet(BLE_CONN_PORT, BLE_CONN_PIN);
}

void BleModule_Process(void)
{
    if (moduleWorkMode == Ble_NoConnect)
    {
        if (GetBleStaPin())
        {
            moduleWorkMode = Ble_Connected;
            // CL_LOG_INFO("ble connect");
            SetMcuLedStyle(McuLedStyle_Bright);
        }
    }
    else if (moduleWorkMode == Ble_Connected)
    {
        if (!GetBleStaPin())
        { // 连接断开
            moduleWorkMode = Ble_NoConnect;
            // CL_LOG_INFO("ble disconnect");
            SetMcuLedStyle(McuLedStyle_NormalBlink);
        }
    }
}

BleModuleWorkMode_t BleModule_GetWorkMode(void)
{
    return moduleWorkMode;
}

