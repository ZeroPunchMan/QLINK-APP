#include "heater.h"
#include "systime.h"
#include "board.h"
#include "ble_module.h"

typedef struct
{
    bool work;
    bool protect;
    uint32_t prtTime;
} HeaterContext_t;

static HeaterContext_t context = {
    .work = false,
    .protect = false,
    .prtTime = 0,
};

static inline void EnableHeater(bool en)
{
    if (en)
        LL_GPIO_ResetOutputPin(HEAT_PORT, HEAT_PIN);
    else
        LL_GPIO_SetOutputPin(HEAT_PORT, HEAT_PIN);
}

static inline bool GetOverheat(void)
{
    return LL_GPIO_IsInputPinSet(HEAT_FB1_PORT, HEAT_FB1_PIN) || LL_GPIO_IsInputPinSet(HEAT_FB2_PORT, HEAT_FB2_PIN);
}

void Heater_Init(void)
{
    EnableHeater(false);
}

void Heater_Process(void)
{
    if (!context.work)
        return;

    if (BleModule_GetWorkMode() != Ble_Connected)
    {
        Heater_SetWork(false);
        return;
    }

    if (context.protect)
    {
        if (SysTimeSpan(context.prtTime) >= SYSTIME_SECOND(30))
        {
            context.protect = false;
            EnableHeater(true);
        }
    }
    else
    {
        if (GetOverheat())
        {
            EnableHeater(false);
            context.protect = true;
            context.prtTime = GetSysTime();
        }
    }
}

void Heater_SetWork(bool w)
{
    context.work = w;

    if (w)
    {
        context.protect = false;
        EnableHeater(true);
    }
    else
    {
        EnableHeater(false);
    }
}
