/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "systime.h"
#include "cl_log.h"
#include "cl_event_system.h"
#include "buzzer.h"
#include "comm.h"
#include "board.h"
#include "flash_layout.h"
#include "led.h"
#include "heater.h"
#include "channel.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

CL_Result_t EraseFlash(uint32_t addr, uint32_t pages)
{
  FLASH_EraseInitTypeDef EraseInitStruct;
  EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
  EraseInitStruct.PageAddress = addr;
  EraseInitStruct.NbPages = pages;

  uint32_t PAGEError = 0;
  if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK)
    return CL_ResFailed;

  return CL_ResSuccess;
}

CL_Result_t WriteFlash(uint32_t addr, const uint8_t *buff, uint32_t length)
{
  uint32_t writeAddr, offset;
  uint32_t data;
  HAL_StatusTypeDef status;

  /* Clear All pending flags */
  // __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR | FLASH_FLAG_PROGERR);

  // little endian [0-0][0-1][0-2][0-3][1-0][1-1][1-2][1-3] -> 3210
  offset = 0;
  writeAddr = addr;
  while (offset < length)
  {
    data = 0;
    for (int i = 0; i < 4; i++)
    {
      if (offset < length)
        data |= (uint32_t)buff[offset++] << (i * 8);
      else
        break;
    }
    status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, writeAddr, data);
    if (status != HAL_OK)
      return CL_ResFailed;

    writeAddr += 4;
  }
  return CL_ResSuccess;
}

void MarkNeedDfu(void)
{
  HAL_FLASH_Unlock();
  EraseFlash(DFU_FLAG_ADDR, 1);

  uint32_t mark[2] = {0x12345678, 0x87654321};
  WriteFlash(DFU_FLAG_ADDR, (const uint8_t *)mark, sizeof(mark));
  HAL_FLASH_Lock();
}

typedef void (*pFunction)(void);
pFunction JumpToAddrFunc;
void JumpToBootloader(void)
{
  uint32_t JumpAddress; // 跳转地址

  JumpAddress = *(volatile uint32_t *)(FLASH_START_ADDR + 4); // 获取复位地址
  JumpToAddrFunc = (pFunction)JumpAddress;                    // 函数指针指向复位地址
  __set_MSP(*(volatile uint32_t *)FLASH_START_ADDR);          // 设置主堆栈指针MSP指向升级机制IAP_ADDR
  JumpToAddrFunc();                                           // 跳转到升级代码处
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
  volatile uint32_t *pVector = (volatile uint32_t *)0x20000000;
  for (uint32_t i = 0; i < 50; i++)
  {
    pVector[i] = ((volatile uint32_t *)APP_START_ADDR)[i];
  }
  LL_SYSCFG_SetRemapMemory(LL_SYSCFG_REMAP_SRAM);

  CL_EventSysInit();
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM14_Init();
  MX_USART1_UART_Init();
  MX_TIM16_Init();
  MX_TIM17_Init();
  MX_TIM1_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
  Pwm_SetCompare(PwmChan_Chan1Amp, 0);
  HAL_TIM_PWM_Start(&htim14, TIM_CHANNEL_1);

  Pwm_SetCompare(PwmChan_Chan1Freq, 0);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);

  Pwm_SetCompare(PwmChan_Chan2Amp, 0);
  HAL_TIM_PWM_Start(&htim17, TIM_CHANNEL_1);

  Pwm_SetCompare(PwmChan_Chan2Freq, 0);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);

  Pwm_SetCompare(PwmChan_Chan3Amp, 0);
  HAL_TIM_PWM_Start(&htim16, TIM_CHANNEL_1);

  Pwm_SetCompare(PwmChan_Chan3Freq, 0);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  Buzzer_Init();
  Comm_Init();
  Led_Init();
  Heater_Init();
  Channel_Init();
  while (1)
  {
    Buzzer_Process();
    Comm_Process();
    Led_Process();
    Heater_Process();
    Channel_Process();
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    static uint32_t lastTime = 0;
    if (SysTimeSpan(lastTime) >= 1000)
    {
      lastTime = GetSysTime();
    }

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);
  while(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_1)
  {
  }
  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {

  }
  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI_DIV_2, LL_RCC_PLL_MUL_12);
  LL_RCC_PLL_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL_IsReady() != 1)
  {

  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {

  }
  LL_SetSystemCoreClock(48000000);

   /* Update the time base */
  if (HAL_InitTick (TICK_INT_PRIORITY) != HAL_OK)
  {
    Error_Handler();
  }
  LL_RCC_SetUSARTClockSource(LL_RCC_USART1_CLKSOURCE_PCLK1);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
