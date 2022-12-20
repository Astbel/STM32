#ifndef __SYSTEMSETTING_H_
#define __SYSTEMSETTING_H_

/*here is for include header files*/
#include "stm32f4xx.h"
#include "main.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "variable.h"
#include "function.h"
#include "math.h"
/*******************Noitce*******************************/
// 所有變數宣告在這只是擴充實際變數宣告致該c檔中
/*******************模組宣告**************************/
extern ADC_HandleTypeDef hadc1;

extern TIM_HandleTypeDef htim1;  //PWM MASTER
extern TIM_HandleTypeDef htim2;  //PMW Salver
extern TIM_HandleTypeDef htim10; //ISR

/*Boolean*/
#define True (1)
#define False (0)

/********************目標命令*********************************/
// 測試用 預計用3.3V去測試
#define Bwron_in_point (0x0279) //(1.75*4095)/3.3 =2172  (0x087C)
#define Bwron_out_point (0x022B)
/***************************RTOS_TASK_CREATE***************************************************/
/*ISR*/
#define Timer_PRESCALER_VALUE (uint32_t)(((SystemCoreClock) / 45000000) - 1)
#define Timer_PERIOD_VALUE (uint32_t)(10500 - 1)          /* Period Value  */
/*PWM Freq & DUTY*/
#define PRESCALER_VALUE (uint32_t)(((SystemCoreClock) / 45000000) - 1)
#define PERIOD_VALUE (uint32_t)(1000 - 1)          /* Period Value  */
#define PULSE1_VALUE (uint32_t)(1000 / 2)          /* Capture Compare 1 Value  */
#define PULSE2_VALUE (uint32_t)(1000 * 37.5 / 100) /* Capture Compare 2 Value  */
#define PULSE3_VALUE (uint32_t)(1000 / 4)          /* Capture Compare 3 Value  */
#define PULSE4_VALUE (uint32_t)(1000 * 12.5 / 100) /* Capture Compare 4 Value  */
/*PWM DUTY MAX MIN*/
/********根據電壓變動限制DUTY開的大小*********/
/*
Vin =127(90Vac~264Vac),VBulk Max 為400V
*Dmax =400/127 =1/1-Dmax    Dmax= 0.6825
*Dmin =400/373 =1/1-Dmin    Dmin= 0.0675
*/
#define MAX_DUTY  (680)
#define MIN_DUTY  (67)
/* Error 點設定 */
/*
*  以115Vac電容抽乾為例換算RMS後Bulk起點電壓為162V 目標命令啟動為380V ,計算其誤差量為380-162=218V
*  Q12格式下設定最大值是400V換算後最大誤差量為(218*4096/400)=2231轉成HEX為008B7
*  設定115Vac下的判斷點
*/
#define Error_limit (0x08B7)  
/*AC 正負半周*/
#define Postive (1)
#define Negative (0)
#define ADC_Sample_Rate (128)
#define ADC_DC_OffSet  (0x6D)  /*0.06offset=(0.6*4095)/3.3*/
/*AC Vmax 定義*/
// #define Vac_max (394)/*264*1.414*/
// #define Vac_min (127)/*90*1.414*/
/*ADC 項目*/
#define AC_N_CHANNEL (0)
#define AC_L_CHANNEL (1)
#define VBUS_CHANNEL (2)

/*數位控制補償參數*/
/*電壓目標命令*/
// #define Vref (0x01000)     //測試用目標命令3.3V
#define Vref (0x0612)  //VBus目標命令 280Vac for 115Vac
// #define Vref (0x0835)     //Vbus 電壓設定380V
//PID I GAIN (MAX)  穩態誤差
#define I_MAX (0x1000)
#define I_MIN (0x0000) 
//PFC switching Freq
#define T (1/85e3)

/*ADC OVP Point*/
// #define OVer_Voltage_VBULK (0x08C0)//(405*4096/450)
#define OVer_Voltage_VBULK (0x067C)//VBUS 300V OVP
#endif
