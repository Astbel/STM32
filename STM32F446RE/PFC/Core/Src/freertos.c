/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
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
// #include "FreeRTOS.h"
// #include "task.h"
#include "main.h"
#include "systemsetting.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/*RTOS Task define here*/
TaskHandle_t TASK_1_Handle;
TaskHandle_t	TASK_2_Handle;
/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
	
/* USER CODE END FunctionPrototypes */
void TEST_GPIO(void);
/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */
void start_task( void * pvParameters )
 {
		xTaskCreate( (TaskFunction_t) Task_1,
                 (char *        ) "Task_1", 
                 (uint16_t 			) TASK_1_SIZE,
                 (void * 				)	NULL,
                 (UBaseType_t   ) TASK_1_PRO,
                 (TaskHandle_t *) &TASK_1_Handle );
								 
    xTaskCreate( (TaskFunction_t) Task_2,
                 (char *        ) "Task_2", 
                 (uint16_t 			) TASK_2_SIZE,
                 (void * 				)	NULL,
                 (UBaseType_t   ) TASK_2_PRO,
                 (TaskHandle_t *) &TASK_2_Handle );	
		vTaskDelete(START_TASK_Handle);
    taskEXIT_CRITICAL();            				 
 }
/* Private application code --------------------------------------------------*/

/*計算*/
void Task_1( void * pvParameters )
{
	 while(1)
	 {
      /*Test Pin here check RTOS is working*/
		 	HAL_GPIO_TogglePin(LD2_GPIO_Port,LD2_Pin);   /*OBser pin for deceted MCU is working*/
      /**Here is adc polling**/
       HAL_GPIO_WritePin(PGI_GPIO_PORT,Power_GOOD_PIN,GPIO_PIN_SET);

      ADC_Sample();
      // Multi_ADC_Sample();
      /*AC  正負半周判斷*/
      // rectify_vac();
			vTaskDelay(20);
	 }
}
 
 /*狀態機切換*/
void Task_2( void * pvParameters )
{
	 while(1)
	 {
      // Uart_sendstring("hi",pc_uart);
      // PFC_TASK_STATE();
      // TEST_GPIO();
			vTaskDelay(100);
	 }
}
/* USER CODE END Application */
void TEST_GPIO(void)
{
  HAL_GPIO_WritePin(PGI_GPIO_PORT,Power_GOOD_PIN,GPIO_PIN_SET);
  HAL_Delay(10);
  HAL_GPIO_WritePin(PGI_GPIO_PORT,Power_GOOD_PIN,GPIO_PIN_RESET);
  HAL_Delay(10);
}

/*Task for sample infine loop*/
/*
1. voltage_Feed_Forward
2. Shift GEARS
3. Dynamic system for feedback loop
4. 
5.
*/
