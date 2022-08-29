/**
  ******************************************************************************
  * @file           : flash.h
  * @brief          : Headers to unctions to send messages through USART1
  * ----------------------------------------------------------------------------
  * @author         : Sebastian Popa
  * @date           : Jan, 2021
  ******************************************************************************
  */

#ifndef INC_FLASH_H_
#define INC_FLASH_H_
#include "main.h"
/* Private defines------------ -----------------------------------------------*/
#define ADDR_FLASH_PAGE_0     ((uint32_t)0x08000000) /* Base @ of Page 0, 2 Kbytes */
#define ADDR_FLASH_PAGE_1     ((uint32_t)0x08000800) /* Base @ of Page 1, 2 Kbytes */
#define ADDR_FLASH_PAGE_2     ((uint32_t)0x08001000) /* Base @ of Page 2, 2 Kbytes */
#define ADDR_FLASH_PAGE_3     ((uint32_t)0x08001800) /* Base @ of Page 3, 2 Kbytes */
#define ADDR_FLASH_PAGE_4     ((uint32_t)0x08002000) /* Base @ of Page 4, 2 Kbytes */
#define ADDR_FLASH_PAGE_5     ((uint32_t)0x08002800) /* Base @ of Page 5, 2 Kbytes */
#define ADDR_FLASH_PAGE_6     ((uint32_t)0x08003000) /* Base @ of Page 6, 2 Kbytes */
#define ADDR_FLASH_PAGE_7     ((uint32_t)0x08003800) /* Base @ of Page 7, 2 Kbytes */
#define ADDR_FLASH_PAGE_8     ((uint32_t)0x08004000) /* Base @ of Page 8, 2 Kbytes */
#define ADDR_FLASH_PAGE_9     ((uint32_t)0x08004800) /* Base @ of Page 9, 2 Kbytes */
#define ADDR_FLASH_PAGE_10    ((uint32_t)0x08005000) /* Base @ of Page 10, 2 Kbytes */
#define ADDR_FLASH_PAGE_11    ((uint32_t)0x08005800) /* Base @ of Page 11, 2 Kbytes */
#define ADDR_FLASH_PAGE_12    ((uint32_t)0x08006000) /* Base @ of Page 12, 2 Kbytes */
#define ADDR_FLASH_PAGE_13    ((uint32_t)0x08006800) /* Base @ of Page 13, 2 Kbytes */
#define ADDR_FLASH_PAGE_14    ((uint32_t)0x08007000) /* Base @ of Page 14, 2 Kbytes */
#define ADDR_FLASH_PAGE_15    ((uint32_t)0x08007800) /* Base @ of Page 15, 2 Kbytes */
#define ADDR_FLASH_PAGE_16    ((uint32_t)0x08008000) /* Base @ of Page 16, 2 Kbytes */
#define ADDR_FLASH_PAGE_17    ((uint32_t)0x08008800) /* Base @ of Page 17, 2 Kbytes */
#define ADDR_FLASH_PAGE_18    ((uint32_t)0x08009000) /* Base @ of Page 18, 2 Kbytes */
#define ADDR_FLASH_PAGE_19    ((uint32_t)0x08009800) /* Base @ of Page 19, 2 Kbytes */
#define ADDR_FLASH_PAGE_20    ((uint32_t)0x0800A000) /* Base @ of Page 20, 2 Kbytes */


/* Private function prototypes -----------------------------------------------*/
/**************************uart_send_msg****************************************
 **
  * @brief	flash Function
  * @param  Erase and Write Data
  * @retval	status
  */
extern void Flash_Data(uint32_t Start_Addr,uint32_t End_Addr,uint32_t write_data);
extern void Erase_Data(uint32_t Start_Addr,uint32_t End_Addr,uint32_t write_data);
extern void UART_Send_Data(uint32_t Start_Addr,uint32_t End_Addr,uint32_t write_data);

//User define the Memory Addr
#define FLASH_USER_START_ADDR  ADDR_FLASH_PAGE_4   /* Start @ of user Flash area */
#define FLASH_USER_END_ADDR     (ADDR_FLASH_PAGE_5 + FLASH_PAGE_SIZE - 1)   /* End @ of user Flash area */
#define FLASH_RX_START_ADDR  ADDR_FLASH_PAGE_7   /* Start @ of user Flash area */
#define FLASH_RX_END_ADDR     (ADDR_FLASH_PAGE_10 + FLASH_PAGE_SIZE - 1)   /* End @ of user Flash area */
//
#define FLASH_UARTSEND1_START_ADDR  ADDR_FLASH_PAGE_11   /* Start @ of user Flash area */
#define FLASH_UARTSEND1_END_ADDR     (ADDR_FLASH_PAGE_12 + FLASH_PAGE_SIZE - 1)   /* End @ of user Flash area */
#define FLASH_UARTSEND2_START_ADDR  ADDR_FLASH_PAGE_21   /* Start @ of user Flash area */
#define FLASH_UARTSEND2_END_ADDR     (ADDR_FLASH_PAGE_22 + FLASH_PAGE_SIZE - 1)   /* End @ of user Flash area */

//Flash Data here 
#define Initial_DATA               ((uint32_t)0x8456237)
#define RX_DATA_32                 ((uint32_t)0x4633222)
#define Write_Data                 ((uint32_t)0x12345)
#define Write_Data_2                ((uint32_t)0x98765)
#endif /* INC_FLASH_H_ */

/***************************END OF FILE****************************************/
