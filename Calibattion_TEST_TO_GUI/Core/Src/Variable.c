#include "main.h"
struct PFC_VARIABLES PFC_Variables;
// 申明變數

uint32_t Protect_12V,Protect_5V;
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
}
