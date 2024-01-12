#include "main.h"

//*************蓝牙模块****************
#define BLE_RST_PORT (GPIOA)
#define BLE_RST_PIN (LL_GPIO_PIN_5)

#define BLE_PWR_PORT (GPIOB)
#define BLE_PWR_PIN (LL_GPIO_PIN_2)

//***************电源控制*******************
#define PWR_12V_EN_PORT (GPIOB)
#define PWR_12V_EN_PIN (LL_GPIO_PIN_9)

#define PWR_5V_EN_PORT (GPIOB)
#define PWR_5V_EN_PIN (LL_GPIO_PIN_8)

//***************按键**********************
#define KEY1_PORT (GPIOB)
#define KEY1_PIN (LL_GPIO_PIN_7)

#define KEY2_PORT (GPIOB)
#define KEY2_PIN (LL_GPIO_PIN_6)

#define KEY3_PORT (GPIOB)
#define KEY3_PIN (LL_GPIO_PIN_5)

//******************语音模块*************************
#define VOICE_MOD_EN_PORT (GPIOB)
#define VOICE_MODE_EN_PIN (LL_GPIO_PIN_3)

//******************风扇*************************
#define FAN_EN_PORT (GPIOF)
#define FAN_EN_PIN (LL_GPIO_PIN_7)

//******************LED*************************
#define LED1_EN_PORT (GPIOA)
#define LED1_EN_PIN (LL_GPIO_PIN_8)

#define LED1_EN_PORT (GPIOB)
#define LED1_EN_PIN (LL_GPIO_PIN_15)

#define LED1_EN_PORT (GPIOB)
#define LED1_EN_PIN (LL_GPIO_PIN_14)

#define LED1_EN_PORT (GPIOB)
#define LED1_EN_PIN (LL_GPIO_PIN_13)
