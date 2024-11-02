#include "comm.h"
#include "cl_event_system.h"
#include "cl_log.h"
#include "cl_serialize.h"
#include "firmware_info.h"
#include "systime.h"
#include "ble_module.h"
#include "buzzer.h"
#include "stdlib.h"
#include "usart.h"
#include "main.h"
#include "channel.h"
#include "heater.h"
#include "buzzer.h"

typedef struct
{
    bool gotKey;
    uint8_t key;
} CommContext_t;

static CommContext_t context = {
    .gotKey = false,
};

void MarkNeedDfu(void);

static void FwUpgradeCheck(uint8_t byte)
{
    const char cmd[] = "FWUPGRADE";
    static uint8_t count = 0;

    if (byte == cmd[count])
    {
        if (++count >= sizeof(cmd) - 1)
        {
            DelayOnSysTime(100);
            MarkNeedDfu();
            DelayOnSysTime(100);
            NVIC_SystemReset();
        }
    }
    else
    {
        count = 0;
    }
}

static void ParseKeyPack(const uint8_t *pack)
{
    uint8_t origin;
    switch (pack[0])
    {
    case 1:
        origin = pack[1];
        break;
    case 2:
        origin = pack[2];
        break;
    case 3:
        origin = pack[3];
        break;
    default:
        return;
    }

    static const uint8_t decTable[8] = {6, 9, 16, 25, 36, 49, 64, 9};
    context.gotKey = true;
    context.key = 0;
    for (int i = 0; i < 8; i++)
    {
        if (origin & (1 << i))
        {
            context.key += decTable[i];
        }
    }
}

static void ParseNormPack(const uint8_t *pack)
{
    if (pack[0] != context.key)
        return;

    if (pack[3] != (pack[1] * 3 + pack[2] * 2))
        return;

    switch (pack[1])
    {
    case 0x08: //
        Buzzer_SetBeep(1);
        Channel_SetEnergy(ChanIdx_1, pack[2]);
        break;
    case 0x0c:
        Buzzer_SetBeep(1);
        Channel_SetEnergy(ChanIdx_2, pack[2]);
        break;
    case 0x12:
        Buzzer_SetBeep(1);
        Channel_SetEnergy(ChanIdx_3, pack[2]);
        break;
    case 0x1c:
        Buzzer_SetBeep(1);
        Heater_SetWork(pack[2]);
        break;
    }
}

static void ParseRecv(uint8_t byte)
{
    static uint32_t lastTime = 0;
    static uint8_t count = 0;
    static uint8_t recvBuff[4];
    if (SysTimeSpan(lastTime) >= 300)
        count = 0;

    lastTime = GetSysTime();
    recvBuff[count++] = byte;
    if (count >= 4)
    {
        if (context.gotKey)
        {
            ParseNormPack(recvBuff);
        }
        else
        {
            ParseKeyPack(recvBuff);
        }

        count = 0;
    }
}

void Comm_Init(void)
{
    BleModule_Init();
}

void Comm_Process(void)
{
    BleModule_Process();

    if (BleModule_GetWorkMode() != Ble_Connected)
    {
        context.gotKey = false;
    }
    else
    {
        if (!context.gotKey)
        {
            static uint32_t lastTime = 0;
            if (SysTimeSpan(lastTime) >= 500)
            {
                lastTime = GetSysTime();

                static const uint8_t prompt[4] = {0xdc, 0x02, 0x02, 0xdd};
                Usartx_Send(USART1, prompt, 0, sizeof(prompt));
            }
        }
    }

    for (int i = 0; i < 100; i++)
    {
        uint8_t data;
        if (Usart1_PollRecvByte(&data) == CL_ResSuccess)
        {
            FwUpgradeCheck(data);
            // 处理应用协议
            ParseRecv(data);
        }
        else
        {
            break;
        }
    }
}
