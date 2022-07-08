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
#include "main.h"
#include "cmsis_os.h"




//Function def here
void StartDefaultTask(void *argument);
void StartTask02(void *argument);
void RTOS_Thread_Init(void);
void Send_Log_System_use(void);
	/* Definitions for MyTask01 */
	osThreadId_t MyTask01Handle;
	const osThreadAttr_t MyTask01_attributes = {
  .name = "MyTask01",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 128 * 4
  };
	/* Definitions for myTask02 */
	osThreadId_t myTask02Handle;
	const osThreadAttr_t myTask02_attributes = {
  .name = "myTask02",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 128 * 4
   };




void RTOS_Thread_Init(void)
{
   /* creation of MyTask01 */
  MyTask01Handle = osThreadNew(StartDefaultTask, NULL, &MyTask01_attributes);

  /* creation of myTask02 */
  myTask02Handle = osThreadNew(StartTask02, NULL, &myTask02_attributes);
}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the MyTask01 thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
    HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_5);
	 if (IsDataAvailable())
      {
         
         int data_Receive=Uart_read();
         UART_Send_Data(FLASH_UARTSEND1_START_ADDR,FLASH_UARTSEND1_END_ADDR,data_Receive);
         Uart_sendstring("Flash Done");
      }
    
    osDelay(100);
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_StartTask02 */
/**
* @brief Function implementing the myTask02 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask02 */
void StartTask02(void *argument)
{
  /* USER CODE BEGIN StartTask02 */
  /* Infinite loop */
  for(;;)
  {
		 if (IsDataAvailable())
      {
         int data=Uart_read();
         Uart_write(data);
      }
    osDelay(1);
  }
  /* USER CODE END StartTask02 */
}


