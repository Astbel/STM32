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
extern float val,Rxval;
extern int number;
extern char string[100];
extern uint32_t Rx_Data[30],data2[9];
extern char *data;
/********************STRUCT***************************/
struct PFC_VARIABLES
{
    uint32_t adc_raw[5];
};
struct Allert_Portect
{
    uint32_t Protect_12V_Max;
    uint32_t Protect_12V_Min;
    uint32_t Portect_5V_max;
    uint32_t Portect_5V_min;
};

//保護點設置
extern uint32_t Protect_12V,Protect_5V;
/*command 結構體*/
// 定义命令-处理函数映射表
typedef struct {
    const char *commandName;
    CommandHandler handler;
} CommandEntry;

/*********************Flag***************************/

/*********************Counter**********************/


#endif
