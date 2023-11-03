#ifndef __FUNCTION_H_
#define __FUNCTION_H_


/**ADC**/
extern void Multi_ADC_Sample(void);
/**Inital Variable**/
extern inline void Initail_Variable(void);
/**Flash**/
extern void Flash_Init(void);
extern void Flash_Erase_Flash_Memory(void);
extern void Flash_Write_Flash_Memory(void);
/****/




// void Uart_send_ADC_Result(struct PFC_VARIABLES *pfc_vars);
#endif