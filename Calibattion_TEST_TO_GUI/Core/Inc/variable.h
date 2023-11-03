#ifndef __VARIABLE_H_
#define __VARIABLE_H_
/************************include**************/

/******************結構體***********************/

extern struct PFC_VARIABLES PFC_Variables;
extern struct Allert_Portect Dyanmic_Portect;

/*********************ADC*****************************/

/**********************C# command 指標函數************************/
typedef void (*CommandHandler)(void);
/***********************Flash**********************/
extern uint32_t FirstSector, NbOfSectors, Address, SECTORError;
extern __IO uint32_t data32, MemoryProgramStatus;
/********************STRUCT***************************/
struct PFC_VARIABLES
{
    uint32_t adc_raw[5];
};
typedef struct Allert_Portect
{
    uint16_t Protect_12V;
    uint16_t Portect_5V;
};

/*command 結構體*/
// 定义命令-处理函数映射表
typedef struct {
    const char *commandName;
    CommandHandler handler;
} CommandEntry;

/*********************Flag***************************/

/*********************Counter**********************/


#endif
