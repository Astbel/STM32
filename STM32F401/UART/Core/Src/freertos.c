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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
osThreadId myTask01Handle;
osThreadId myTask02Handle;
osThreadId myTask03Handle;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */
int buf[30];
/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void Mqtt_connect(char*A,char *MQTTID, char *Server_Host,char *PORT);
/* USER CODE END FunctionPrototypes */
void RTOS_Initliaze(void);
/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );
void StartDefaultTask(void const * argument);
void StartTask02(void const * argument);
void StartTask03(void const * argument);
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
void RTOS_Initliaze(void)
{
  osThreadDef(myTask01, StartDefaultTask, osPriorityNormal, 0, 128);
  myTask01Handle = osThreadCreate(osThread(myTask01), NULL);

  /* definition and creation of myTask02 */
  osThreadDef(myTask02, StartTask02, osPriorityIdle, 0, 128);
  myTask02Handle = osThreadCreate(osThread(myTask02), NULL);

  /* definition and creation of myTask03 */
  osThreadDef(myTask03, StartTask03, osPriorityIdle, 0, 128);
  myTask03Handle = osThreadCreate(osThread(myTask03), NULL);
}



/*  ------------------RTOS Task Here--------------------------------*/
/* USER CODE BEGIN Application */
/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the myTask01 thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
    HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_5);
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
void StartTask02(void const * argument)
{
  /* USER CODE BEGIN StartTask02 */
  /* Infinite loop */
  for(;;)
  {
     if (IsDataAvailable(pc_uart))
	  {
		  int data = Uart_read(pc_uart);
		  Uart_write(data, device_uart);
	  }

	  if (IsDataAvailable(device_uart))
	  {
		  // if (Get_after("AT version:", 8, buffer, wifi_uart))
		  // {
			//   Uart_sendstring("AT VERSION=", pc_uart);
			//   Uart_sendstring(buffer, pc_uart);
		  // }

		  int data = Uart_read(device_uart);
		  Uart_write(data, pc_uart);
	  }
    osDelay(200);
  }
  /* USER CODE END StartTask02 */
}

/* USER CODE BEGIN Header_StartTask03 */
/**
* @brief Function implementing the myTask03 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask03 */
void StartTask03(void const * argument)
{
  /* USER CODE BEGIN StartTask03 */
  /* Infinite loop */
  for(;;)
  {
    Mqtt_connect("4","TCP,","test.mosquitto.org,","1883");
    osDelay(200);
  }
  /* USER CODE END StartTask03 */
}
/* USER CODE END Application */
//Test Buffer
void mqtt_buff(void)
{
buf[0] = 16;   //mqtt連線判別byte
buf[1] = 28;
buf[2] = 0x00;
buf[3] = 6;//協定長度(MQIsdp 所以是6)
buf[4] = 0x4d;//(這邊是ASCII) M
buf[5] = 0x51;//(這邊是ASCII) Q
buf[6] = 0x49;//(這邊是ASCII) I
buf[7] = 0x73;//(這邊是ASCII) s
buf[8] = 0x64;//(這邊是ASCII) d
buf[9] = 0x70;//(這邊是ASCII) p
buf[9] = 0x03 ;//(MQTT 版本  3就是 3.0版  4是3.1版)
buf[10]= 0xC2;  //(實際上後面運算是有設定帳號密碼才要但是這邊設了也沒差)
buf[11]= 0x00 ;// (keep Alive Time High byte)
buf[12]= 0x30 ;//(keep Alive Time Low byte)
buf[13]= 0x00 ;// (自訂ID長度 High Byte)
buf[14]= 0x0A ;//(自訂ID長度 Low Byte)
buf[15]= 0x41;//"A"
buf[16]= 0x73;//"s"
buf[17]= 0x74;//"t"
buf[18]= 0x62;//"b"
buf[19]= 0x65;//"e"
buf[20]= 0x6c;//"l"
buf[21]= 0x54;//"T"
buf[22]= 0x65;//"e"
buf[23]= 0x73;//"s"
buf[24]= 0x54;//"T"
buf[25]= 0x41;//"A"
buf[26]= 0x42;//"B"
buf[27]= 0x43;//"C"
buf[28]= 0x41;//"A"
buf[29]= 0x42;//"B"
buf[30]= 0x43;//"C"
}

//Connect to MQtt server
void Mqtt_connect(char*A,char *MQTTID, char *Server_Host,char *PORT)
{
  char mqtt_data[80];
  /********* AT+CIPMUX=1 **********/
	Uart_sendstring("AT+CIPMUX=1\r\n", device_uart);
	while (!(Wait_for("AT+CIPMUX=1\r\r\n\r\nOK\r\n", device_uart)));
	Uart_sendstring("set as multiconnect mode\n\n", pc_uart);

  /********* AT+CIPSTART **********/
  Uart_sendstring("connecting... to mqtt broker\n", pc_uart);
  sprintf (mqtt_data,"AT+CIPSTART=\"%s\",\"%s\",\"%s\",\"%s\"\r\n",A,MQTTID,Server_Host,PORT);
  Uart_sendstring(mqtt_data, device_uart);
  while (!(Wait_for("OK\r\n", device_uart)));
  Uart_sendstring("STM32 Connect server complete\n\n", pc_uart);

}

