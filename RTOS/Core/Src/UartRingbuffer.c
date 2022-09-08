

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
uint32_t MAX_DUTY_Calculate;
uint32_t ARR_LAST_TIME_SAVE;
//Multi channel here
ring_buffer rx_buffer1 = { { 0 }, 0, 0};
ring_buffer tx_buffer1 = { { 0 }, 0, 0};
ring_buffer rx_buffer2 = { { 0 }, 0, 0};
ring_buffer tx_buffer2 = { { 0 }, 0, 0};

ring_buffer *_rx_buffer1;
ring_buffer *_tx_buffer1;
ring_buffer *_rx_buffer2;
ring_buffer *_tx_buffer2;


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
  _rx_buffer1 = &rx_buffer1;
  _tx_buffer1 = &tx_buffer1;
  _rx_buffer2 = &rx_buffer2;
  _tx_buffer2 = &tx_buffer2;

  /* Enable the UART Error Interrupt: (Frame error, noise error, overrun error) */
  __HAL_UART_ENABLE_IT(device_uart,UART_IT_ERR);
  __HAL_UART_ENABLE_IT(wifi_uart,UART_IT_ERR);
  /* Enable the UART Data Register not empty Interrupt */
  __HAL_UART_ENABLE_IT(device_uart,UART_IT_RXNE);
  __HAL_UART_ENABLE_IT(wifi_uart,UART_IT_RXNE);
}

//TEST stored data
void store_char(unsigned char c, ring_buffer *buffer)
{
	int i=(unsigned int)(buffer->head +1) % UART_BUFFER_SIZE;

	buffer->buffer[buffer->head]=c;
	 
	buffer->head=i;
}

int Copy_upto (char *string, char *buffertocopyinto, UART_HandleTypeDef *uart)
{
	int so_far =0;
	int len = strlen (string);
	int indx = 0;

again:
	while (!IsDataAvailable(uart));
	while (Uart_peek(uart) != string[so_far])
		{
			buffertocopyinto[indx] = _rx_buffer1->buffer[_rx_buffer1->tail];
			_rx_buffer1->tail = (unsigned int)(_rx_buffer1->tail + 1) % UART_BUFFER_SIZE;
			indx++;
			while (!IsDataAvailable(uart));

		}
	while (Uart_peek(uart) == string [so_far])
	{
		so_far++;
		buffertocopyinto[indx++] = Uart_read(uart);
		if (so_far == len) return 1;
		while (!IsDataAvailable(uart));
	}

	if (so_far != len)
	{
		so_far = 0;
		goto again;
	}

	if (so_far == len) return 1;
	else return -1;

}

int Get_after (char *string, uint8_t numberofchars, char *buffertosave, UART_HandleTypeDef *uart)
{

	while (Wait_for(string, uart) != 1);
	for (int indx=0; indx<numberofchars; indx++)
	{
		while (!(IsDataAvailable(uart)));
		buffertosave[indx] = Uart_read(uart);
	}
	return 1;
}

void Uart_flush (UART_HandleTypeDef *uart)
{
	if (uart == device_uart)
	{
		memset(_rx_buffer1->buffer,'\0', UART_BUFFER_SIZE);
		_rx_buffer1->head = 0;
	}
	if (uart ==  wifi_uart)
	{
		memset(_rx_buffer2->buffer,'\0', UART_BUFFER_SIZE);
		_rx_buffer2->head = 0;
	}
}


int Uart_peek(UART_HandleTypeDef *uart)
{
	if (uart == device_uart)
	{
		  if(_rx_buffer1->head == _rx_buffer1->tail)
		  {
		    return -1;
		  }
		  else
		  {
		    return _rx_buffer1->buffer[_rx_buffer1->tail];
		  }
	}

	else if (uart ==  wifi_uart)
	{
		  if(_rx_buffer2->head == _rx_buffer2->tail)
		  {
		    return -1;
		  }
		  else
		  {
		    return _rx_buffer2->buffer[_rx_buffer2->tail];
		  }
	}

	return -1;
}

int Look_for (char *str, char *buffertolookinto)
{
	int stringlength = strlen (str);
	int bufferlength = strlen (buffertolookinto);
	int so_far = 0;
	int indx = 0;
repeat:
	while (str[so_far] != buffertolookinto[indx]) indx++;
	if (str[so_far] == buffertolookinto[indx]){
	while (str[so_far] == buffertolookinto[indx])
	{
		so_far++;
		indx++;
	}
	}

	else
		{
			so_far =0;
			if (indx >= bufferlength) return -1;
			goto repeat;
		}

	if (so_far == stringlength) return 1;
	else return -1;
}


int Uart_read(UART_HandleTypeDef *uart)
{
	if (uart == device_uart)
	{
		  // if the head isn't ahead of the tail, we don't have any characters
		  if(_rx_buffer1->head == _rx_buffer1->tail)
		  {
		    return -1;
		  }
		  else
		  {
		    unsigned char c = _rx_buffer1->buffer[_rx_buffer1->tail];
		    _rx_buffer1->tail = (unsigned int)(_rx_buffer1->tail + 1) % UART_BUFFER_SIZE;
		    return c;
		  }
	}

	else if (uart == wifi_uart)
	{
		  // if the head isn't ahead of the tail, we don't have any characters
		  if(_rx_buffer2->head == _rx_buffer2->tail)
		  {
		    return -1;
		  }
		  else
		  {
		    unsigned char c = _rx_buffer2->buffer[_rx_buffer2->tail];
		    _rx_buffer2->tail = (unsigned int)(_rx_buffer2->tail + 1) % UART_BUFFER_SIZE;
		    return c;
		  }
	}

	else return -1;
}

void Uart_write(int c, UART_HandleTypeDef *uart)
{
	if ((c>=0)&&(uart==device_uart))
	{
		int i = (_tx_buffer1->head + 1) % UART_BUFFER_SIZE;

		_tx_buffer1->buffer[_tx_buffer1->head] = (uint8_t)c;
		_tx_buffer1->head = i;

		__HAL_UART_ENABLE_IT(device_uart, UART_IT_TXE); // Enable UART transmission interrupt
	}
	if ((c>=0)&&(uart==wifi_uart))
	{
		int i = (_tx_buffer2->head + 1) % UART_BUFFER_SIZE;
		
		_tx_buffer2->buffer[_tx_buffer2->head] = (uint8_t)c;
		_tx_buffer2->head = i;

		__HAL_UART_ENABLE_IT(wifi_uart, UART_IT_TXE); // Enable UART transmission interrupt
			
	}
	
}

int IsDataAvailable(UART_HandleTypeDef *uart)
{
	if (uart == device_uart) return (uint16_t)(UART_BUFFER_SIZE + _rx_buffer1->head - _rx_buffer1->tail) % UART_BUFFER_SIZE;
	else if (uart == wifi_uart) return (uint16_t)(UART_BUFFER_SIZE + _rx_buffer2->head - _rx_buffer2->tail) % UART_BUFFER_SIZE;
	return -1;
}
void Uart_sendstring (const char *s, UART_HandleTypeDef *uart)
{
	while(*s!='\0') Uart_write(*s++, uart);
}

int Wait_for (char *string,UART_HandleTypeDef *uart)
{
	int so_far =0;
	int len = strlen (string);

again_device:
	while (!IsDataAvailable(uart));
	if (Uart_peek(uart) != string[so_far])
	{
		 _rx_buffer1->tail = (unsigned int)(_rx_buffer1->tail + 1) % UART_BUFFER_SIZE ;
		goto again_device;

	}
	while (Uart_peek(uart) == string [so_far])
	{
		so_far++;
		Uart_read(uart);
		if (so_far == len) return 1;
		while (!IsDataAvailable(uart));
	}

	if (so_far != len)
	{
		so_far = 0;
		goto again_device;
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
		 if (huart == device_uart)
        {
        	store_char (c, _rx_buffer1);  // store data in buffer
        }

        else if (huart == wifi_uart)
        {
           	store_char (c, _rx_buffer2);  // store data in buffer
        }
	   
	 	time_out_cnt=200;

        //判斷目前接收的字串長度,當head 以及計算長度相等時則為收完資料
		// Check_Length(_rx_buffer);
		
	    return;
    }
    
    /*If interrupt is caused due to Transmit Data Register Empty */
   if (((isrflags & USART_ISR_TXE_TXFNF) != RESET) && ((cr1its & USART_CR1_TXEIE_TXFNFIE) != RESET))
    {
    	if (huart == device_uart){
    	if(tx_buffer1.head == tx_buffer1.tail)
    	    {
    	      // Buffer empty, so disable interrupts
    	      __HAL_UART_DISABLE_IT(huart, UART_IT_TXE);

    	    }

    	 else
    	    {
    	      // There is more data in the output buffer. Send the next byte
    	      unsigned char c = tx_buffer1.buffer[tx_buffer1.tail];
    	      tx_buffer1.tail = (tx_buffer1.tail + 1) % UART_BUFFER_SIZE;

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
    	}

    	else if (huart == wifi_uart){
        	if(tx_buffer2.head == tx_buffer2.tail)
        	    {
        	      // Buffer empty, so disable interrupts
        	      __HAL_UART_DISABLE_IT(huart, UART_IT_TXE);

        	    }

        	 else
        	    {
        	      // There is more data in the output buffer. Send the next byte
        	      unsigned char c = tx_buffer2.buffer[tx_buffer2.tail];
        	      tx_buffer2.tail = (tx_buffer2.tail + 1) % UART_BUFFER_SIZE;

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
  if ((_rx_buffer1->buffer[0]=='D')&&(_rx_buffer1->buffer[1]=='u')&&(_rx_buffer1->buffer[2]=='t')&&(_rx_buffer1->buffer[3]=='y')&&(_rx_buffer1->buffer[5]=='\0')&&(_rx_buffer1->buffer[6]=='\0')&&(_rx_buffer1->buffer[7]=='\0'))
  {
	 Search_String(rx_buffer1.buffer,output_Buff,4,1);
     Str_PWM  =atoi(output_Buff);
	 //跟新PWM
	// PWM_Duty=((Str_PWM*MAX_DUTY)/MAX_DUTY_percentage)+0x032;
	PWM_Duty=((Str_PWM*MAX_DUTY_Calculate)/MAX_DUTY_percentage)+0x032;
    TIM1->CCR1=PWM_Duty;
  }
  //PWM  十位數
  else if((_rx_buffer1->buffer[0]=='D')&&(_rx_buffer1->buffer[1]=='u')&&(_rx_buffer1->buffer[2]=='t')&&(_rx_buffer1->buffer[3]=='y')&&(_rx_buffer1->buffer[6]=='\0')&&(_rx_buffer1->buffer[7]=='\0'))
  {
	 Search_String(rx_buffer1.buffer,output_Buff,4,2);
     Str_PWM  =atoi(output_Buff);
	 //跟新PWM
	// PWM_Duty=((Str_PWM*MAX_DUTY)/MAX_DUTY_percentage)+0x032;
	PWM_Duty=((Str_PWM*MAX_DUTY_Calculate)/MAX_DUTY_percentage)+0x032;
    TIM1->CCR1=PWM_Duty;
  }
   //PWM  百位數
  else if((_rx_buffer1->buffer[0]=='D')&&(_rx_buffer1->buffer[1]=='u')&&(_rx_buffer1->buffer[2]=='t')&&(_rx_buffer1->buffer[3]=='y')&&(_rx_buffer1->buffer[7]=='\0'))
  {
	 Search_String(rx_buffer1.buffer,output_Buff,4,3);
     Str_PWM  =atoi(output_Buff);
	 //跟新PWM
	// PWM_Duty=((Str_PWM*MAX_DUTY)/MAX_DUTY_percentage);
	PWM_Duty=((Str_PWM*MAX_DUTY_Calculate)/MAX_DUTY_percentage)+0x032;
    TIM1->CCR1=PWM_Duty;
  }
  //Freq
  else if ((_rx_buffer1->buffer[0]=='F')&&(_rx_buffer1->buffer[1]=='r')&&(_rx_buffer1->buffer[2]=='e')&&(_rx_buffer1->buffer[3]=='q'))
  {
	Search_String(rx_buffer1.buffer,output_Buff,4,4);
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
  rx_buffer1.head=0;
  rx_buffer1.tail=0;
  memset(_rx_buffer1->buffer,'\0', UART_BUFFER_SIZE);
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



