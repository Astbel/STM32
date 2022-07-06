#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "main.h"
#include "string.h"

void Send_Log_System_use(void);

void Send_Log_System_use(void)
{
    uint16_t cnt_ms = 0;
    
    vTaskDelay(10);
        cnt_ms++;
        if(cnt_ms >99)
            cnt_ms = 0;
    if(cnt_ms == 0)
			{
				 uint8_t *cpu_run_info;
				 cpu_run_info = pvPortMalloc(TASK_LOG_NUM);
				if(cpu_run_info != NULL)
				{
					memset(cpu_run_info,0,TASK_LOG_NUM); //信息缓冲区清零
					vTaskList((char *)cpu_run_info); //获取任务运行时间信息
					APP_LOG_DEBUG("---------------------------------------------\r\n");
					APP_LOG_DEBUG("TASK NAME \t\tTask_status   poritiy   stack   task_number\r\n");
					APP_LOG_DEBUG("%s", cpu_run_info);
					APP_LOG_DEBUG("---------------------------------------------\r\n");
					memset(cpu_run_info,0,TASK_LOG_NUM); //信息缓冲区清零
					vTaskGetRunTimeStats((char *)cpu_run_info);
					APP_LOG_DEBUG("Task name \t\toperate_count         using_rate\r\n");
					APP_LOG_DEBUG("%s", cpu_run_info);
					APP_LOG_DEBUG("---------------------------------------------\r\n");
					APP_LOG_DEBUG("Stack_size: %dByte\r\n",xPortGetFreeHeapSize());
					APP_LOG_DEBUG("Unchecking_History_Stack_memory_size :%dByte\r\n\n",xPortGetMinimumEverFreeHeapSize());
					vPortFree(cpu_run_info);
				}
			}
}
