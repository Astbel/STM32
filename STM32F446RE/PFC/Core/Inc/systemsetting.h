#ifndef __SYSTEMSETTING_H_
#define __SYSTEMSETTING_H_

/*here is for include header files*/
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "UartRingbuffer_multi.h"
#include "FreeRTOS.h"
#include "task.h"


/*Define*/
#define device_uart &huart1
#define pc_uart &huart3
/*Variable delcare here*/

/*RTOS Task define here*/
#define START_TSK_SIZE 128
#define START_TASK_PRO 1
extern TaskHandle_t START_TASK_Handle;

#define TASK_1_SIZE 128
#define TASK_1_PRO 2
// TaskHandle_t TASK_1_Handle;

#define TASK_2_SIZE 128
#define TASK_2_PRO 3
// TaskHandle_t	TASK_2_Handle;

/*RTOS Sub Function */
void start_task( void * pvParameters );
void Task_1( void * pvParameters );
void Task_2( void * pvParameters );

/*Task scheduler for mian loop*/
void start_task( void * pvParameters );

#endif
