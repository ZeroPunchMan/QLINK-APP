#include "channel.h"
#include "ble_module.h"
#include "tim.h"
#include "systime.h"
#include "usart.h"

static const uint8_t energyTable[50] = { // 0~240
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
    11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
    21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
    31, 32, 33, 34, 35, 36, 36, 36, 36, 36,
    36, 36, 36, 36, 36, 36, 36, 36, 36, 36};

typedef enum
{
    ChanFreq_High = 1,
    ChanFreq_Low = 2,
} ChanFreq_t;

typedef struct
{
    uint8_t energy[ChanIdx_Max];
    ChanFreq_t freq;
    uint32_t freqTime;
    uint16_t timPeriod; // 550~1100
    uint16_t targetPeriod;
} ChanContext_t;

static volatile ChanContext_t context;

static bool IsAnyChanWork(void)
{
    for (int i = 0; i < ChanIdx_Max; i++)
    {
        if (context.energy[i] > 0)
            return true;
    }
    return false;
}

static void SetFreq(ChanFreq_t freq)
{
    context.freq = freq;
    context.freqTime = GetSysTime();
    if (freq == ChanFreq_Low)
    {
        static const uint8_t prompt[4] = {0x58, 0x58, 0x02, 0x58};
        Usartx_Send(USART1, prompt, 0, sizeof(prompt));
        context.targetPeriod = LOW_FREQ_PERIOD;
    }
    else
    {
        static const uint8_t prompt[4] = {0x58, 0x58, 0x01, 0x58};
        Usartx_Send(USART1, prompt, 0, sizeof(prompt));
        context.targetPeriod = HIGH_FREQ_PERIOD;
    }
}

static void FreqSmooth(void)
{
    static uint32_t lastTime = 0;
    if (SysTimeSpan(lastTime) < 2)
        return;

    lastTime = GetSysTime();

    if (context.timPeriod > context.targetPeriod)
        context.timPeriod--;
    else if (context.timPeriod < context.targetPeriod)
        context.timPeriod++;

    context.timPeriod = CL_CLAMP(context.timPeriod, HIGH_FREQ_PERIOD, LOW_FREQ_PERIOD);
    Pwm_SetCounterPeriod(context.timPeriod);
}

void Channel_Init(void)
{
    for (int i = 0; i < ChanIdx_Max; i++)
        context.energy[i] = 0;

    context.freq = ChanFreq_High;
    context.freqTime = 0;
    context.timPeriod = LOW_FREQ_PERIOD;
    context.targetPeriod = LOW_FREQ_PERIOD;
}

void Channel_Process(void)
{
    if (IsAnyChanWork())
    {
        if (BleModule_GetWorkMode() != Ble_Connected)
        { // 蓝牙断开后关闭全部通道
            for (int i = 0; i < ChanIdx_Max; i++)
                Channel_SetEnergy((ChanIdx_t)i, 0);
            return;
        }
        else
        {
            if (SysTimeSpan(context.freqTime) >= SYSTIME_SECOND(30))
            { // 30s调频率
                if (context.freq == ChanFreq_High)
                    SetFreq(ChanFreq_Low);
                else if (context.freq == ChanFreq_Low)
                    SetFreq(ChanFreq_High);
            }

            // 平滑调整
            FreqSmooth();
        }
    }
}

void Channel_SetEnergy(ChanIdx_t chan, uint8_t energy)
{
    energy = CL_MIN(energy, CL_ARRAY_LENGTH(energyTable));

    if (!IsAnyChanWork())
        SetFreq(ChanFreq_Low);

    context.energy[(int)chan] = energy;

    PwmChannel_t ampPwmChan;
    PwmChannel_t freqPwmChan;
    switch (chan)
    {
    case ChanIdx_1:
        ampPwmChan = PwmChan_Chan1Amp;
        freqPwmChan = PwmChan_Chan1Freq;
        break;
    case ChanIdx_2:
        ampPwmChan = PwmChan_Chan2Amp;
        freqPwmChan = PwmChan_Chan2Freq;
        break;
    case ChanIdx_3:
        ampPwmChan = PwmChan_Chan3Amp;
        freqPwmChan = PwmChan_Chan3Freq;
        break;
    }

    if (energy)
    {
        Pwm_SetCompare(ampPwmChan, energyTable[energy - 1]);
        Pwm_SetCompare(freqPwmChan, 20);
    }
    else
    {
        Pwm_SetCompare(ampPwmChan, 0);
        Pwm_SetCompare(freqPwmChan, 0);
    }
}
