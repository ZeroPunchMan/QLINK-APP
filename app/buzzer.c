#include "buzzer.h"
#include "systime.h"
#include "cl_log.h"
#include "board.h"

typedef struct
{
    bool beep;
    uint32_t stepTime;
    uint8_t count;
} BeepContext_t;

static BeepContext_t context = {
    .beep = false,
    .stepTime = 0,
    .count = 0,
};

static inline void BeepEnable(bool en)
{
    if (en)
        LL_GPIO_SetOutputPin(BUZZ_PORT, BUZZ_PIN);
    else
        LL_GPIO_ResetOutputPin(BUZZ_PORT, BUZZ_PIN);
}

void Buzzer_Init(void)
{
    BeepEnable(false);
}

void Buzzer_Process(void)
{
    if (context.beep)
    {
        if (SysTimeSpan(context.stepTime) >= BEEP_DURATION)
        {
            context.beep = false;
            context.stepTime = GetSysTime();
            BeepEnable(false);
            if (context.count)
                context.count--;

            // CL_LOG_INFO("beep done, rem: %d", context.count);
        }
    }
    else
    {
        if (SysTimeSpan(context.stepTime) >= BEEP_INTERVAL)
        {
            if (context.count)
            {
                context.beep = true;
                context.stepTime = GetSysTime();
                BeepEnable(true);
                // CL_LOG_INFO("beep cont, rem: %d", context.count);
            }
        }
    }
}

void Buzzer_SetBeep(uint8_t count)
{
    context.beep = true;
    context.stepTime = GetSysTime();
    context.count = count;
    BeepEnable(true);
    // CL_LOG_INFO("set beep, count: %d", context.count);
}

void Buzzer_Stop(void) 
{
    context.beep = false;
    context.count = 0;
    BeepEnable(false);
}
