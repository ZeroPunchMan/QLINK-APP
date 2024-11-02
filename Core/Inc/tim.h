/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    tim.h
  * @brief   This file contains all the function prototypes for
  *          the tim.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIM_H__
#define __TIM_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern TIM_HandleTypeDef htim1;

extern TIM_HandleTypeDef htim3;

extern TIM_HandleTypeDef htim14;

extern TIM_HandleTypeDef htim16;

extern TIM_HandleTypeDef htim17;

/* USER CODE BEGIN Private defines */
typedef enum
{
  PwmChan_Chan1Amp = 0, //amp cmp 0~240
  PwmChan_Chan1Freq, //freq period 550~1100 cmp 20
  PwmChan_Chan2Amp,
  PwmChan_Chan2Freq,
  PwmChan_Chan3Amp,
  PwmChan_Chan3Freq,
} PwmChannel_t;
/* USER CODE END Private defines */

void MX_TIM1_Init(void);
void MX_TIM3_Init(void);
void MX_TIM14_Init(void);
void MX_TIM16_Init(void);
void MX_TIM17_Init(void);

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* USER CODE BEGIN Prototypes */
void Pwm_SetCompare(PwmChannel_t channel, uint32_t D);
void Pwm_SetCounterPeriod(uint16_t p);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __TIM_H__ */

