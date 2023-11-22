#ifndef __VARIABLE_H_
#define __VARIABLE_H_
/************************include**************/

/******************結構體***********************/

extern struct PFC_VARIABLES PFC_Variables;
extern struct Allert_Portect Dyanmic_Portect;
extern struct Flash_Dynamic Flash_Memory;
/*********************ADC*****************************/

/**********************C# command 指標函數************************/
typedef void (*CommandHandler)(void);
/***********************Flash**********************/
// 測試函數 變數宣告
extern float val, Rxval;
extern int number;
extern char string[100];
extern uint32_t Rx_Data[30], data2[9];
extern char *data;
// 正式使用函數 變數宣告
extern uint32_t Min_5V_Data;
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
// Flash 結構體
struct Flash_Dynamic
{
    /*兩點校正參數*/
    float block_c_5V;
    float block_b_5V;
    float block_a_5V;
    float block_c_12V;
    float block_b_12V;
    float block_a_12V;
    /*記憶體儲存位置*/
    uint32_t adc_value_5V_min;
    uint32_t adc_value_5V_max;
    uint32_t adc_value_12V_min;
    uint32_t adc_value_12V_max;
    uint32_t adc_value_3_3V_min;
    uint32_t adc_value_3_3V_max;
    /*兩點校正輸出*/
    float Slope_Value_5V;
    float Slope_Value_12V;
};

// 保護點設置
extern uint32_t Protect_12V, Protect_5V;
/*command 結構體*/
// 定义命令-处理函数映射表
typedef struct
{
    const char *commandName;
    CommandHandler handler;
} CommandEntry;

/*********************Flash***************************/
extern int check_data;
// Addr for memory
extern uint32_t Data_5V_Min_Addr;
extern uint32_t Data_5V_Max_Addr;
extern uint32_t Data_12V_Min_Addr;
extern uint32_t Data_12V_Max_Addr;
/*********************Slope method**********************/
extern float Slope_value;

#endif
