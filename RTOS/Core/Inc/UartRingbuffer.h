
#ifndef UARTRINGBUFFER_H_
#define UARTRINGBUFFER_H_

// #include "stm32f4xx_hal.h"
#include "stm32g4xx_hal.h"
#include <string.h>
/*Multi Uart Define here*/
#define  device_uart &huart1
#define  test_uart   &huart2
#define  wifi_uart   &huart3
/* change the size of the buffer */
#define UART_BUFFER_SIZE 8
#define BUFFER_SIZE 4
typedef struct
{
  unsigned char buffer[UART_BUFFER_SIZE];
  volatile unsigned int head;
  volatile unsigned int tail;
} ring_buffer;


// extern volatile char output_Buff[BUFFER_SIZE];
//Search string buffer
extern void Check_Status(void);
extern uint16_t Str_PWM;
extern uint32_t Str_Freq;
extern char Targert_Buff[BUFFER_SIZE];
/*確認字串結束傳送*/
// int Check_Data_Receive(ring_buffer *buffer)
/*字串搜尋*/
void Search_String(char s[],char out[],uint16_t p,uint16_t l);
//Method 確認長度
unsigned int Check_Length(ring_buffer *buffer);
/* Initialize the ring buffer */
void Ringbuf_init(void);

/* reads the data in the rx_buffer and increment the tail count in rx_buffer */
int Uart_read(UART_HandleTypeDef *uart);

/* writes the data to the tx_buffer and increment the head count in tx_buffer */
void Uart_write(int c, UART_HandleTypeDef *uart);

/* function to send the string to the uart */
void Uart_sendstring (const char *s, UART_HandleTypeDef *uart);

/* Print a number with any base
 * base can be 10, 8 etc*/
void Uart_printbase (long n, uint8_t base);

/* Checks if the data is available to read in the rx_buffer */
int IsDataAvailable(UART_HandleTypeDef *uart);

/* Copies the required data from a buffer
 * @startString: the string after which the data need to be copied
 * @endString: the string before which the data need to be copied
 * @USAGE:: GetDataFromBuffer ("name=", "&", buffertocopyfrom, buffertocopyinto);
 */
void GetDataFromBuffer (char *startString, char *endString, char *buffertocopyfrom, char *buffertocopyinto);

/* Resets the entire ring buffer, the new data will start from position 0 */
void Uart_flush (UART_HandleTypeDef *uart);
int Uart_peek(UART_HandleTypeDef *uart);
/* Wait until a paricular string is detected in the Rx Buffer
* Return 1 on success and -1 otherwise
* USAGE: while (!(Wait_for("some string")));
*/
int Wait_for (char *string,UART_HandleTypeDef *uart);


/* the ISR for the uart. put it in the IRQ handler */
void Uart_isr (UART_HandleTypeDef *huart);



/*** Depreciated For now. This is not needed, try using other functions to meet the requirement ***/
/* get the position of the given string within the incoming data.
 * It returns the position, where the string ends 
 */
//uint16_t Get_position (char *string);

/* once you hit 'enter' (\r\n), it copies the entire string to the buffer*/
//void Get_string (char *buffer);


#endif /* UARTRINGBUFFER_H_ */
