
#include "systemsetting.h"
/*Variable*/
uint16_t analog_result_volt;
uint16_t ADC_Result[ADC_Sample_Rate];
float VBulk;
uint32_t test_ac_sample;
uint32_t ac_sum;
float real_ac;
uint16_t Sample_cnt;
uint16_t Vac_temp;
uint16_t Vac_PP;
uint16_t Vac_filter;

/*ADC 取樣 5 Channel*/
/*
1. VBulk
2. Phase_A_IL
3. Phase_B_IL
4. Vac_L
5. Vac_N
*/
uint16_t Vmax, Vmin;
/*Function define*/



/*ADC Select Channel*/
/**ADC1 GPIO Configuration
PA0-WKUP     ------> ADC1_IN0   Vac_N
PA1     ------> ADC1_IN1        Vac_L
PA6     ------> ADC1_IN6        Vbulk
*/
// Vac_N
void ADC_Select_CH0(void)
{
    ADC_ChannelConfTypeDef sConfig = {0};
    /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
     */
    sConfig.Channel = ADC_CHANNEL_0;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }
}
// Vac_L
void ADC_Select_CH1(void)
{
    ADC_ChannelConfTypeDef sConfig = {0};
    /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
     */
    sConfig.Channel = ADC_CHANNEL_1;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }
}
// VBulk
void ADC_Select_CH2(void)
{
    ADC_ChannelConfTypeDef sConfig = {0};
    /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
     */
    sConfig.Channel = ADC_CHANNEL_4;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }
}
// IL_PhaseA


/*Task for sample multi data*/
void Multi_ADC_Sample(void)
{
    /*************************************************************************************/
    ADC_Select_CH0();
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, 1000);
    PFC_Variables.adc_raw[0] = HAL_ADC_GetValue(&hadc1);
    // ADC_SAMPLE_ARR[AC_N_CHANNEL] = HAL_ADC_GetValue(&hadc1);
    HAL_ADC_Stop(&hadc1);
    /*************************************************************************************/
    ADC_Select_CH1();
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, 1000);
    PFC_Variables.adc_raw[1] = HAL_ADC_GetValue(&hadc1);
    // ADC_SAMPLE_ARR[AC_L_CHANNEL] = HAL_ADC_GetValue(&hadc1);
    HAL_ADC_Stop(&hadc1);
    /*************************************************************************************/
    ADC_Select_CH2();
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, 1000);
    PFC_Variables.adc_raw[2] = HAL_ADC_GetValue(&hadc1);
    // VBulk = (float)((PFC_Variables.adc_raw[2] * 400) / 4096);
    VBulk = (float)((PFC_Variables.adc_raw[2] * 405) / 2240);
    HAL_ADC_Stop(&hadc1);
    /*************************************************************************************/
}
/*Kp=0.3  Ki=1*/
/***************************開機初始化函數***********************************/
inline void Initail_Variable(void)
{
    /*PFC inital STATE*/
    PFC_Variables.supply_state = STATE_IDLE;
    /*PID 初始化設定*/
    /*PID  正常誤差量*/
    PID.kp = 0x04CD; // 0.3  轉Q12
    PID.ki = 0x0FFF; // 1
    //TEST PI form PSIM
    // PID.kp = 0x226; // 0.15  轉Q15
    // PID.ki = 0x199; // 0.1

    /*PID Vref 誤差量很大的設定*/
    PID.Kp_limit = 0x03FF; // Q12  0.25
    PID.Ki_limit = 0x006;  // Q12  0.0015

    /*Vac 變數初始化設定*/
    Vac_Bwron_in_Cnt = 0;
    Vac_peak_temp = 0;
    Vac_peak = 0;
}
/*ISR Function*/


