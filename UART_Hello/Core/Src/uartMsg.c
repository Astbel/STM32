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
char my_uart_buffer[256];
int my_uart_buffer_index = 0;
uint8_t aRxMessage[1];
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
	Status = HAL_UART_Transmit_IT(&huart1, (uint8_t*)user_data,
			strlen(user_data));

	return Status;
#endif
}

HAL_StatusTypeDef uart_send_INT(int *user_data)
{
#ifndef USE_UART
	// do nothing
	return HAL_OK;
#else
	HAL_StatusTypeDef Status;
	Status = HAL_UART_Transmit_IT(&huart1, (uint8_t*)user_data,
			sizeof(user_data));

	return Status;
#endif
}


void uart1_handler(void){
  char buff; 
  HAL_UART_Receive_IT (&huart1, (uint8_t *)&buff, 1);
  my_uart_buffer[my_uart_buffer_index++] = buff;
  // uart_send_INT(&buff);
  // uart_send_msg(&buff);

  // uart_send_msg(buff);
}

void uart1_idleHandler(){
  my_uart_buffer_index = 0; 
 
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
	HAL_UART_Receive_IT(&huart1,aRxMessage,1);
	if(aRxMessage[0]=='1')
	{
      uart_send_msg("OK");		
	}
	else if(aRxMessage[0]=='2')
	{
		 uart_send_msg("FUCK U");	
	}
  else
  {
     uart_send_msg("SO Bad");	
  }
	memset(aRxMessage,0,sizeof(aRxMessage));  //清除字串

}


/***************************END OF FILE****************************************/
