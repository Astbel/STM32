#include "UartRingbuffer_INT.h"
#include "main.h"



//Initail

Ring_Buffer_Int RX_buffer={ {0}, 0, 0};
Ring_Buffer_Int TX_buffer={ {0}, 0, 0};

Ring_Buffer_Int *_RX_buffer;
Ring_Buffer_Int *_TX_buffer;

void RingBuf_init(void)
{
  _RX_buffer =&RX_buffer;
  _TX_buffer =&TX_buffer;

  /* Enable the UART Error Interrupt: (Frame error, noise error, overrun error) */
  __HAL_UART_ENABLE_IT(&huart1, UART_IT_ERR);

  /* Enable the UART Data Register not empty Interrupt */
  __HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);

}

//store data
void store_data(unsigned int c, ring_buffer *buffer)
{
  int i = (unsigned int)(buffer->head + 1) % Buffer_Size;

  // if we should be storing the received character into the location
  // just before the tail (meaning that the head would advance to the
  // current location of the tail), we're about to overflow the buffer
  // and so we don't write the character or advance the head.
  if(i != buffer->tail) {
    buffer->buffer[buffer->head] = c;
    buffer->head = i;
  }
}
//
int Uart_read_INT(void)
{
  // if the head isn't ahead of the tail, we don't have any characters
  if(_RX_buffer->in == _RX_buffer->out)
  {
    return -1;
  }
  else
  {
    unsigned int c = _RX_buffer->buffer[__RX_buffer->out];
    _RX_buffer->out = (unsigned int)(_RX_buffer->out + 1) % Buffer_Size;
    return c;
  }
}

void Uart_write_INT(int c)
{
	if (c>=0)
	{
		int i = (_TX_buffer->in + 1) % Buffer_Size;

		// If the output buffer is full, there's nothing for it other than to
		// wait for the interrupt handler to empty it a bit
		// ???: return 0 here instead?
		while (i == _TX_buffer->out);

		_TX_buffer->buffer[_TX_buffer->in] = (uint8_t)c;
		_TX_buffer->in = i;

		__HAL_UART_ENABLE_IT(&huart1, UART_IT_TXE); // Enable UART transmission interrupt
	}
}


//ISR
void Uart_isr_INT (UART_HandleTypeDef *huart)
{
	  uint32_t isrflags   = READ_REG(huart->Instance->ISR);
	  uint32_t cr1its     = READ_REG(huart->Instance->CR1);

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
        store_data (c, _RX_buffer);  // store data in buffer
        return;
    }

    /*If interrupt is caused due to Transmit Data Register Empty */
    if (((isrflags & USART_ISR_TXE_TXFNF) != RESET) && ((cr1its & USART_CR1_TXEIE_TXFNFIE) != RESET))
    {
    	if(TX_buffer.in == TX_buffer.out)
    	    {
    	      // Buffer empty, so disable interrupts
    	      __HAL_UART_DISABLE_IT(huart, UART_IT_TXE);

    	    }

    	 else
    	    {
    	      // There is more data in the output buffer. Send the next byte
    	      unsigned char c = TX_buffer.buffer[TX_buffer.out];
    	      TX_buffer.out = (TX_buffer.out + 1) % UART_BUFFER_SIZE;

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