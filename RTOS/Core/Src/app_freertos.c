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
#include "UartRingbuffer.h"



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
/* UART Receive Task 1ms */
void StartDefaultTask(void *argument)
{
  for(;;)
  {
    HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_5);
    if (time_out_cnt>0)
    {

      time_out_cnt--;
      if (time_out_cnt==0)
      {
         // time_out_flag=0;     //下一輪
          Data_Flag=1;        //資料全部收完
      }
    }

    osDelay(1);
  }
}

/* USER CODE BEGIN Header_StartTask02 */
/**
* @brief Function implementing the myTask02 thread.
* @param argument: Not used
* @retval None
*/
/* Check UART Buffer */
void StartTask02(void *argument)
{
  /* USER CODE BEGIN StartTask02 */
  /* Infinite loop */
  for(;;)
  {
		if (Data_Flag==1)
		{
			Check_Status();
		}
    //Test uart3
      Uart_sendstring("Check UART3 is working",device_uart);
    osDelay(100);
  }
  /* USER CODE END StartTask02 */
}


