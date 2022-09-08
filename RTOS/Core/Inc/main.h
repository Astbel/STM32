/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g4xx_hal.h"
#include "Flash.h"
#include "Variable.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "ESP8266_HAL.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#define APP_LOG_DEBUG(...)  printf2(__VA_ARGS__)
/* USER CODE END Includes */
/* Compute the prescaler value to have TIM1 counter clock equal to 85000000 Hz */

#define PRESCALER_VALUE     (uint32_t)(((SystemCoreClock) / 85000000) - 1)
/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
  // typedef int8_t               int8;
	// typedef uint8_t              uint8;
	// typedef int16_t              int16;
	// typedef uint16_t             uint16;
	// typedef int32_t              int32;
	// typedef uint32_t             uint32;
	// typedef int64_t              int64;
	// typedef uint64_t             uint64;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
typedef enum
{
  Phase_1,
  Phase_2,
  Phase_3,
  Phase_4
}PWM_STATE;

/* USER CODE END EC */
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern TIM_HandleTypeDef htim1;
/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);
extern uint16_t PWM_Duty;
extern uint32_t MAX_DUTY_Calculate;
extern uint32_t ARR_LAST_TIME_SAVE;
/*Uart Time out variable*/
extern uint16_t time_out_cnt;        //單筆timeout計數      //timeout 旗標
extern uint8_t  Data_Flag;          //整組資料旗標
/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */
#define  PERIOD_VALUE       (uint32_t)(1000 - 1)              /* Period Value  */
#define  PULSE1_VALUE       (uint32_t)(1000 / 2)              /* Capture Compare 1 Value  */
#define  PULSE2_VALUE       (uint32_t)(1000 * 37.5 / 100)     /* Capture Compare 2 Value  */
#define  PULSE3_VALUE       (uint32_t)(1000 / 4)              /* Capture Compare 3 Value  */
#define  PULSE4_VALUE       (uint32_t)(1000 * 12.5 /100)      /* Capture Compare 4 Value  */
/* USER CODE END Private defines */
//UART control duty  HEX value convert
#define  MAX_DUTY             (0x03E8)
#define  MAX_DUTY_percentage  (0x0064)
#define  True  (1)
#define  False (0)
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
