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
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#define APP_LOG_DEBUG(...)  printf2(__VA_ARGS__)
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
  typedef signed char          int8;
	typedef char                 Uint8;
	typedef short                int16;
	typedef unsigned short       Uint16;
	typedef int                  int32;
	typedef unsigned int         Uint32;
	typedef long long            int64;
	typedef unsigned long long   Uint64;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */
extern UART_HandleTypeDef huart1;
/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_HRTIM_MspPostInit(HRTIM_HandleTypeDef *hhrtim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/

#define PWM_FEQ 100000
#define TIMD_DUTY_CYCLE 0.5
#define HRTIM_INPUT_CLOCK 170000000
#define TIMD_PERIOD ((uint16_t)((((uint64_t)HRTIM_INPUT_CLOCK) * 32) / PWM_FEQ))
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
