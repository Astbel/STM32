#ifndef __FUNCTION_H_
#define __FUNCTION_H_


/**ADC**/
extern void Multi_ADC_Sample(void);
/**Inital Variable**/
extern void Initail_Variable(void);
/**Flash**/
extern void Flash_Erase_Sectors(uint32_t startSector, uint32_t endSector);
extern uint32_t Flash_Write_Flash_Memory(uint32_t *data,uint32_t startAddr, uint16_t numberofwords);
extern uint32_t Flash_Find_Data(uint32_t *targetData, uint32_t size, uint32_t flashAddress);
/**Uart command ptr function**/
extern void Get5VMinCommand(void);
extern void Get5VMaxCommand(void);
extern void Get12VMinCommand(void);
extern void Get12VMaxCommand(void);
extern void EraseFlashMemoryCommand(void);
extern void Check_Flash_Memory_Data(void);
extern void Serial_Slopping_Method(void);
// void Uart_send_ADC_Result(struct PFC_VARIABLES *pfc_vars);
#endif