#ifndef UARTRINGBUFFER_INT_H_
#define UARTRINGBUFFER_INT_H_

#include "stm32g4xx_hal.h"

//Buffer Size
#define Buffer_Size  256

typedef struct 
{
    unsigned char buffer[Buffer_Size];
    //pointer
    unsigned char  in;
    unsigned char out;
}Ring_Buffer_Int;

//method
void RingBuf_init(void);
void store_data(unsigned int c, ring_buffer *buffer);
int Uart_read_INT(void);
void Uart_write_INT(int c);
void Uart_isr_INT (UART_HandleTypeDef *huart);
#endif /* UARTRINGBUFFER_H_ */