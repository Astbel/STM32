/**
  ******************************************************************************
  * @file           : uartMsg.c
  * @brief          : Functions to send messages through USART1
  * ----------------------------------------------------------------------------
  * @author         : Sebastian Popa
  * @date           : Jan, 2021
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "main.h"
#include "uartMsg.h"

/* Variables declared elsewhere-----------------------------------------------*/
extern UART_HandleTypeDef huart1; // defined in usart.c
__IO ITStatus UartReady = RESET;

uint8_t rx_buff[10];
//uint8_t tx_buff[10];
//UART中斷 變數宣告
//static unsigned char   uRx_Data[1024] = {0};    //存储数组
//static unsigned char  *pRx_Data= uRx_Data;    //指向存储数组将要存储数据的位
//static unsigned char   uLength= 0;    //接收数据长度

/* Private function prototypes -----------------------------------------------*/
/**************************uart_send_msg****************************************
 **
  * @brief	Sends a message through UART
  * @param	user_data
  * @retval	status
  */
HAL_StatusTypeDef uart_send_msg(char *user_data)
{
#ifndef USE_UART
	// do nothing
	return HAL_OK;
#else
	HAL_StatusTypeDef Status;
	Status = HAL_UART_Transmit(&huart1, (uint8_t*)user_data,
			strlen(user_data), UART_DELAY);

	return Status;
#endif
}
/**
  * @brief  Rx Transfer completed callback
  * @param  UartHandle: UART handle
  * @note   This example shows a simple way to report end of IT Rx transfer, and
  *         you can add your own implementation.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
	HAL_UART_Receive_IT (&huart1, rx_buff, 4); //RESET ISR
	uart_send_msg("receive data\r\n");
}

/***************************END OF FILE****************************************/
