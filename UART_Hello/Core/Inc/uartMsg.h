/**
  ******************************************************************************
  * @file           : uartMsg.h
  * @brief          : Headers to unctions to send messages through USART1
  * ----------------------------------------------------------------------------
  * @author         : Sebastian Popa
  * @date           : Jan, 2021
  ******************************************************************************
  */

#ifndef INC_UARTMSG_H_
#define INC_UARTMSG_H_

#include "main.h"

/* Private defines------------ -----------------------------------------------*/
#define UART_DELAY	100    //Delay in ms
#define USE_UART	1
extern Uint8 Flash_done_Flag;
extern uint8_t rx_buff[10];
extern uint8_t tx_buff;
/* Private function prototypes -----------------------------------------------*/
/**************************uart_send_msg****************************************
 **
  * @brief	Sends a message through UART
  * @param	user_data
  * @retval	status
  */
HAL_StatusTypeDef uart_send_msg(char * user_data);

#endif /* INC_UARTMSG_H_ */

/***************************END OF FILE****************************************/
