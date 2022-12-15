/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : app_freertos.c
 * Description        : Code for freertos applications
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

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "systemsetting.h"

TaskHandle_t TASK_1_Handle;
TaskHandle_t TASK_2_Handle;

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize);

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize)
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}

/* USER CODE END GET_IDLE_TASK_MEMORY */
void start_task(void *pvParameters)
{
  xTaskCreate((TaskFunction_t)Task_1,
              (char *)"Task_1",
              (uint16_t)TASK_1_SIZE,
              (void *)NULL,
              (UBaseType_t)TASK_1_PRO,
              (TaskHandle_t *)&TASK_1_Handle);

  xTaskCreate((TaskFunction_t)Task_2,
              (char *)"Task_2",
              (uint16_t)TASK_2_SIZE,
              (void *)NULL,
              (UBaseType_t)TASK_2_PRO,
              (TaskHandle_t *)&TASK_2_Handle);
  vTaskDelete(START_TASK_Handle);
  taskEXIT_CRITICAL();
}

void Task_1(void *pvParameters)
{
  while (1)
  {
    /*Test Pin here check RTOS is working*/
    HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin); /*OBser pin for deceted MCU is working*/
    /**Here is adc polling**/
    Multi_Sweep_ADC_Sample();
    /*AC  正負半周判斷*/
    // rectify_vac();
    vTaskDelay(20);
  }
}

/*狀態機切換*/
void Task_2(void *pvParameters)
{
  while (1)
  {
    // PFC_TASK_STATE();
    vTaskDelay(100);
  }
}
/*Test RTOS*/



