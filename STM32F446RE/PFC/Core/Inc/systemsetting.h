#ifndef __SYSTEMSETTING_H_
#define __SYSTEMSETTING_H_

/*here is for include header files*/
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "UartRingbuffer_multi.h"
#include "FreeRTOS.h"
#include "task.h"
#include "variable.h"
#include "function.h"

/*******************Noitce*******************************/
// 所有變數宣告在這只是擴充實際變數宣告致該c檔中
/*******************模組宣告**************************/
extern ADC_HandleTypeDef hadc1;

extern DAC_HandleTypeDef hdac;

extern TIM_HandleTypeDef htim10;
extern TIM_HandleTypeDef htim11;

extern UART_HandleTypeDef huart1;  // 打log
extern UART_HandleTypeDef huart3;  // C#調整溝通 

extern TaskHandle_t START_TASK_Handle;
/*Boolean*/
#define True   (1)
#define False  (0)

/********************目標命令*********************************/
//測試用 預計用3.3V去測試
#define Bwron_in_point  (0x087C)  //(1.75*4095)/3.3 =2172  (0x087C)

/***************************RTOS_TASK_CREATE***************************************************/
/*Define*/
#define device_uart &huart1
#define pc_uart &huart3
/*Variable delcare here*/

/*RTOS Task define here*/
#define START_TSK_SIZE 128
#define START_TASK_PRO 1


#define TASK_1_SIZE 128
#define TASK_1_PRO 2
// TaskHandle_t TASK_1_Handle;

#define TASK_2_SIZE 128
#define TASK_2_PRO 3
// TaskHandle_t	TASK_2_Handle;

/*PWM Freq & DUTY*/
#define PRESCALER_VALUE     (uint32_t)(((SystemCoreClock) / 85000000) - 1)
#define  PERIOD_VALUE       (uint32_t)(1000 - 1)              /* Period Value  */
#define  PULSE1_VALUE       (uint32_t)(1000 / 2)              /* Capture Compare 1 Value  */
#define  PULSE2_VALUE       (uint32_t)(1000 * 37.5 / 100)     /* Capture Compare 2 Value  */
#define  PULSE3_VALUE       (uint32_t)(1000 / 4)              /* Capture Compare 3 Value  */
#define  PULSE4_VALUE       (uint32_t)(1000 * 12.5 /100)      /* Capture Compare 4 Value  */


#endif