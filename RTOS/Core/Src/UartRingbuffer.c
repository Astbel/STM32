

#include "UartRingbuffer.h"
#include "main.h"
/**** define the UART you are using  ****/

// UART_HandleTypeDef huart1;

// #define uart &huart1

/* put the following in the ISR 

extern void Uart_isr (UART_HandleTypeDef *huart);

*/

/****************=======================>>>>>>>>>>> NO CHANGES AFTER THIS =======================>>>>>>>>>>>**********************/


char Targert_Buff[BUFFER_SIZE];
uint16_t Str_PWM;
uint32_t Str_Freq;
uint8_t string_length;
ring_buffer rx_buffer = { { 0 }, 0, 0};
ring_buffer tx_buffer = { { 0 }, 0, 0};

ring_buffer *_rx_buffer;
ring_buffer *_tx_buffer;
 char output_Buff[BUFFER_SIZE];
// uint8_t time_out_flag;
//Method
void store_char(unsigned char c, ring_buffer *buffer);
void Check_Status(void);
unsigned int Check_Length(ring_buffer *buffer);

void Ringbuf_init(void)
{
     //Target string set
	 Str_PWM	=0;
	 Str_Freq	=0;
	 string_length=0;
	 memset(output_Buff,'\0', BUFFER_SIZE);
	//
  _rx_buffer = &rx_buffer;
  _tx_buffer = &tx_buffer;

  /* Enable the UART Error Interrupt: (Frame error, noise error, overrun error) */
  __HAL_UART_ENABLE_IT(&huart1, UART_IT_ERR);

  /* Enable the UART Data Register not empty Interrupt */
  __HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
}

// void store_char(unsigned char c, ring_buffer *buffer)
// {
//   int i = (unsigned int)(buffer->head +1) % UART_BUFFER_SIZE;

//   // if we should be storing the received character into the location
//   // just before the tail (meaning that the head would advance to the
//   // current location of the tail), we're about to overflow the buffer
//   // and so we don't write the character or advance the head.
//    if(i != buffer->tail) {
//     buffer->buffer[buffer->head] = c;
//     buffer->head = i;
//  }
// }
//TEST stored data
void store_char(unsigned char c, ring_buffer *buffer)
{
	int i=(unsigned int)(buffer->head +1) % UART_BUFFER_SIZE;

	buffer->buffer[buffer->head]=c;
	 
	buffer->head=i;
}

void Uart_flush (void)
{
	memset(_rx_buffer->buffer,'\0', UART_BUFFER_SIZE);
	_rx_buffer->head = 0;
}

int Uart_peek()
{
  if(_rx_buffer->head == _rx_buffer->tail)
  {
    return -1;
  }
  else
  {
    return _rx_buffer->buffer[_rx_buffer->tail];
  }
}


int Uart_read(void)
{
  // if the head isn't ahead of the tail, we don't have any characters
  if(_rx_buffer->head == _rx_buffer->tail)
  {
    return -1;
  }
  else
  {
    unsigned char c = _rx_buffer->buffer[_rx_buffer->tail];
    _rx_buffer->tail = (unsigned int)(_rx_buffer->tail + 1) % UART_BUFFER_SIZE;
    return c;
  }
}

void Uart_write(int c)
{
	if (c>=0)
	{
		int i = (_tx_buffer->head + 1) % UART_BUFFER_SIZE;

		// If the output buffer is full, there's nothing for it other than to
		// wait for the interrupt handler to empty it a bit
		// ???: return 0 here instead?
		while (i == _tx_buffer->tail);

		_tx_buffer->buffer[_tx_buffer->head] = (uint8_t)c;
		_tx_buffer->head = i;

		__HAL_UART_ENABLE_IT(&huart1, UART_IT_TXE); // Enable UART transmission interrupt
	}
}

int IsDataAvailable(void)
{
  return (uint16_t)(UART_BUFFER_SIZE + _rx_buffer->head - _rx_buffer->tail) % UART_BUFFER_SIZE;
}

void Uart_sendstring (const char *s)
{
	while(*s) Uart_write(*s++);
}

int Wait_for (char *string)
{
	int so_far =0;
	int len = strlen (string);

again:
	while (!IsDataAvailable());
	while (Uart_peek() != string[so_far]) _rx_buffer->tail = (unsigned int)(_rx_buffer->tail + 1) % UART_BUFFER_SIZE;
	while (Uart_peek() == string [so_far])
	{
		so_far++;
		Uart_read();
		if (so_far == len) return 1;
		while (!IsDataAvailable());
	}

	if (so_far != len)
	{
		so_far = 0;
		goto again;
	}

	if (so_far == len) return 1;
	else return -1;
}




void Uart_isr (UART_HandleTypeDef *huart)
{
	  uint32_t isrflags   = READ_REG(huart->Instance->ISR);
	  uint32_t cr1its     = READ_REG(huart->Instance->CR1);
    //TImeout Register
	  static uint8_t   timeout_counter=0;
    /* if DR is not empty and the Rx Int is enabled */
    if (((isrflags & USART_ISR_RXNE_RXFNE) != RESET) && ((cr1its & USART_CR1_RXNEIE_RXFNEIE) != RESET))
    {
    	 /******************
    	    	      *  @note   PE (Parity error), FE (Framing error), NE (Noise error), ORE (Overrun
    	    	      *          error) and IDLE (Idle line detected) flags are cleared by software
    	    	      *          sequence: a read operation to USART_SR register followed by a read
    	    	      *          operation to USART_DR register.
    	    	      * @note   RXNE flag can be also cleared by a read to the USART_DR register.
    	    	      * @note   TC flag can be also cleared by software sequence: a read operation to
    	    	      *          USART_SR register followed by a write operation to USART_DR register.
    	    	      * @note   TXE flag is cleared only by a write to the USART_DR register.

    	 *********************/
    	huart->Instance->ISR;                       /* Read status register */
        unsigned char c = huart->Instance->RDR;     /* Read data register */
	    store_char (c, _rx_buffer); 
	
	 	time_out_cnt=200;
		
		
        //判斷目前接收的字串長度,當head 以及計算長度相等時則為收完資料
		// Check_Length(_rx_buffer);
		
	    return;
    }
    
    /*If interrupt is caused due to Transmit Data Register Empty */
    if (((isrflags & USART_ISR_TXE_TXFNF) != RESET) && ((cr1its & USART_CR1_TXEIE_TXFNFIE) != RESET))
    {
    	if(tx_buffer.head == tx_buffer.tail)
    	    {
    	      // Buffer empty, so disable interrupts
    	      __HAL_UART_DISABLE_IT(huart, UART_IT_TXE);

    	    }

    	 else
    	    {
    	      // There is more data in the output buffer. Send the next byte
    	      unsigned char c = tx_buffer.buffer[tx_buffer.tail];
    	      tx_buffer.tail = (tx_buffer.tail + 1) % UART_BUFFER_SIZE;

    	      /******************
    	      *  @note   PE (Parity error), FE (Framing error), NE (Noise error), ORE (Overrun
    	      *          error) and IDLE (Idle line detected) flags are cleared by software
    	      *          sequence: a read operation to USART_SR register followed by a read
    	      *          operation to USART_DR register.
    	      * @note   RXNE flag can be also cleared by a read to the USART_DR register.
    	      * @note   TC flag can be also cleared by software sequence: a read operation to
    	      *          USART_SR register followed by a write operation to USART_DR register.
    	      * @note   TXE flag is cleared only by a write to the USART_DR register.

    	      *********************/

    	      huart->Instance->ISR;
    	      huart->Instance->TDR = c;

    	    }
    	return;
    }
}

//find C# string method
//
//Parameter:ring_buffer *buffer 接收端主字串
//			char Target[]   目標字串
//
void Search_String(char s[],char out[],uint16_t p,uint16_t l)
{
     int8_t cnt=0;				//取值得計數
	 int8_t out_last_length=l;  //要寫入的位置
	 int8_t j=0;					//補 0的計數
	/*strcspn*/
    char *sTmp;
	//指標致receive端
    sTmp=s;
	//填入Buffer
    while (cnt<l)
	{
		out[out_last_length]=*(sTmp+p+cnt);
	    cnt ++;
		out_last_length++;
	}
	//剩下位數補0
	while (j<l)
	{
		out[j]='0';
		j++;
	}
}


/*
狀態確認  判斷Receive Buffer端的末段結尾來判斷是幾位數
*/
void Check_Status(void)
{
   //PWM  個位數
  if ((_rx_buffer->buffer[0]=='D')&&(_rx_buffer->buffer[1]=='u')&&(_rx_buffer->buffer[2]=='t')&&(_rx_buffer->buffer[3]=='y')&&(_rx_buffer->buffer[5]=='\0')&&(_rx_buffer->buffer[6]=='\0')&&(_rx_buffer->buffer[7]=='\0'))
  {
	 Search_String(rx_buffer.buffer,output_Buff,4,1);
     Str_PWM  =atoi(output_Buff);
	 //跟新PWM
	// PWM_Duty=((Str_PWM*MAX_DUTY)/MAX_DUTY_percentage)+0x032;
	PWM_Duty=((Str_PWM*MAX_DUTY_Calculate)/MAX_DUTY_percentage)+0x032;
    TIM1->CCR1=PWM_Duty;
  }
  //PWM  十位數
  else if((_rx_buffer->buffer[0]=='D')&&(_rx_buffer->buffer[1]=='u')&&(_rx_buffer->buffer[2]=='t')&&(_rx_buffer->buffer[3]=='y')&&(_rx_buffer->buffer[6]=='\0')&&(_rx_buffer->buffer[7]=='\0'))
  {
	 Search_String(rx_buffer.buffer,output_Buff,4,2);
     Str_PWM  =atoi(output_Buff);
	 //跟新PWM
	// PWM_Duty=((Str_PWM*MAX_DUTY)/MAX_DUTY_percentage)+0x032;
	PWM_Duty=((Str_PWM*MAX_DUTY_Calculate)/MAX_DUTY_percentage)+0x032;
    TIM1->CCR1=PWM_Duty;
  }
   //PWM  百位數
  else if((_rx_buffer->buffer[0]=='D')&&(_rx_buffer->buffer[1]=='u')&&(_rx_buffer->buffer[2]=='t')&&(_rx_buffer->buffer[3]=='y')&&(_rx_buffer->buffer[7]=='\0'))
  {
	 Search_String(rx_buffer.buffer,output_Buff,4,3);
     Str_PWM  =atoi(output_Buff);
	 //跟新PWM
	// PWM_Duty=((Str_PWM*MAX_DUTY)/MAX_DUTY_percentage);
	PWM_Duty=((Str_PWM*MAX_DUTY_Calculate)/MAX_DUTY_percentage)+0x032;
    TIM1->CCR1=PWM_Duty;
  }
  //Freq
  else if ((_rx_buffer->buffer[0]=='F')&&(_rx_buffer->buffer[1]=='r')&&(_rx_buffer->buffer[2]=='e')&&(_rx_buffer->buffer[3]=='q'))
  {
	Search_String(rx_buffer.buffer,output_Buff,4,4);
    Str_Freq  =atoi(output_Buff);
	//存入當筆ARR值
	ARR_LAST_TIME_SAVE=TIM1->ARR;
	//跟新Freq
	Str_Freq=Str_Freq*1000;
	TIM1->ARR=(uint32_t)((SystemCoreClock)/((TIM1->PSC+1)*Str_Freq));
	//刷新DUTY對應此狀態下的值
	TIM1->CCR1=((TIM1->CCR1*TIM1->ARR)/ARR_LAST_TIME_SAVE);//等比跟新DUTY
	MAX_DUTY_Calculate=TIM1->ARR;      //跟新最大DUTY
  }
  //重製Head & Tail 
  rx_buffer.head=0;
  rx_buffer.tail=0;
  memset(_rx_buffer->buffer,'\0', UART_BUFFER_SIZE);
  //清除全部收完旗標等待下一次
  Data_Flag=0;
}

//check length  method
unsigned int Check_Length(ring_buffer *buffer)
{
	if (buffer->head>=buffer->tail)
	{
		string_length= (uint8_t)(buffer->head-buffer->tail);
		return string_length;
	}
	else
	{
		string_length=(uint8_t)(UART_BUFFER_SIZE-(buffer->head+buffer->tail));
		return string_length;
	}
}



