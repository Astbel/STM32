#include "main.h"
struct PFC_VARIABLES PFC_Variables;
// 申明變數
char *data = "hello FLASH from Astbel\
			  This is a test to see how many words can we work with";

uint32_t data2[] = {0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9};

uint32_t Rx_Data[30];

char string[100];

int number = 123;

float val = 123.456;

float Rxval;
uint32_t Protect_12V, Protect_5V;

/*Flash 變數申明區*/
uint32_t Min_5V_Data;

/*
 * 初始化變數變量
 * 請區分 所有結構體 為一組 以利於分辨
 *
 */
void Initail_Variable(void)
{
    // inital adc value of array adc
    for (int i = 0; i < 5; i++)
        PFC_Variables.adc_raw[i] = 0;
    // Inital Flash variable

    Protect_12V = 0, Protect_5V = 0;

    /*Flash 測試變數使用區*/
    char *data = "hello FLASH from Astbel\
			  This is a test to see how many words can we work with";

    uint32_t data2[] = {0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9};

    uint32_t Rx_Data[30];

    char string[100];

    int number = 123;

    float val = 123.456;

    float RxVal;

    /*Flash 宣告變數區*/
    Min_5V_Data = 1752;
}
