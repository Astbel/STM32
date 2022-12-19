
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
uint16_t adc_sample1;
uint16_t adc_sample2;
float adc_conv_sample_1;
float adc_conv_sample_2;
float adc_conv_sample_3;
float adc_conv_sample_4;
float adc_conv_sample_5;
/*Loop  Counter */
uint8_t loop_cnt;

/*ADC 取樣 5 Channel*/
/*
1. VBulk
2. Phase_A_IL
3. Phase_B_IL
4. Vac_L
5. Vac_N
*/
uint16_t Vmax, Vmin;

/*ADC Select Channel*/
/**ADC1 GPIO Configuration
    PA0     ------> ADC1_IN1  AC_L
    PA1     ------> ADC1_IN2  AC_N
    PA2     ------> ADC1_IN3  Vbulk
    PA3     ------> ADC1_IN4  IL_A
    PB14    ------> ADC1_IN5 IL_B
    */
void ADC_Select_Channel(uint8_t Channel_value)
{
    switch (Channel_value)
    {
        ADC_ChannelConfTypeDef sConfig = {0};
    /*AC_L*/
    case 1:
        sConfig.Channel = ADC_CHANNEL_1;
        sConfig.Rank = ADC_REGULAR_RANK_1;
        sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
        sConfig.SingleDiff = ADC_SINGLE_ENDED;
        sConfig.OffsetNumber = ADC_OFFSET_NONE;
        sConfig.Offset = 0;
        if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
        {
            Error_Handler();
        }
        break;
    /*AC_N*/
    case 2:
        // sConfig.Channel = ADC_CHANNEL_2;
        sConfig.Channel = ADC_CHANNEL_3;
        sConfig.Rank = ADC_REGULAR_RANK_1;
        sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
        sConfig.SingleDiff = ADC_SINGLE_ENDED;
        sConfig.OffsetNumber = ADC_OFFSET_NONE;
        sConfig.Offset = 0;
        if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
        {
            Error_Handler();
        }
        break;
    /*VBulk*/
    case 3:
        sConfig.Channel = ADC_CHANNEL_3;
        sConfig.Rank = ADC_REGULAR_RANK_1;
        sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
        sConfig.SingleDiff = ADC_SINGLE_ENDED;
        sConfig.OffsetNumber = ADC_OFFSET_NONE;
        sConfig.Offset = 0;
        if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
        {
            Error_Handler();
        }
        break;
    /*IL Phase A*/
    case 4:
        sConfig.Channel = ADC_CHANNEL_4;
        sConfig.Rank = ADC_REGULAR_RANK_1;
        sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
        sConfig.SingleDiff = ADC_SINGLE_ENDED;
        sConfig.OffsetNumber = ADC_OFFSET_NONE;
        sConfig.Offset = 0;
        if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
        {
            Error_Handler();
        }
        break;
        /*IL Phase B*/
    case 5:
        sConfig.Channel = ADC_CHANNEL_5;
        sConfig.Rank = ADC_REGULAR_RANK_1;
        sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
        sConfig.SingleDiff = ADC_SINGLE_ENDED;
        sConfig.OffsetNumber = ADC_OFFSET_NONE;
        sConfig.Offset = 0;
        if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
        {
            Error_Handler();
        }
        break;

    default:
        break;
    }
}

/*ADC Multi Channel sample*/
void Multi_Sweep_ADC_Sample(void)
{
    for (loop_cnt = 1; loop_cnt <=3; loop_cnt++)
    {
        ADC_Select_Channel(loop_cnt);
        HAL_ADC_Start(&hadc1);
        PFC_Variables.adc_raw[loop_cnt-1] = HAL_ADC_GetValue(&hadc1);
        HAL_ADC_Stop(&hadc1);
    }
}
/***************************開機初始化函數***********************************/
inline void Initail_Variable(void)
{
    /*PFC inital STATE*/
    PFC_Variables.supply_state = STATE_IDLE;
    /*PID 初始化設定*/
    /*PID  正常誤差量*/
    PID.kp = 0x4000; // 0.5  轉Q15
    PID.ki = 0x8000; // 2
    // TEST PI form PSIM
    //  PID.kp = 0x226; // 0.15  轉Q15
    //  PID.ki = 0x199; // 0.1

    /*PID Vref 誤差量很大的設定*/
    PID.Kp_limit = 0x03FF; // Q12  0.25
    PID.Ki_limit = 0x006;  // Q12  0.0015

    /*Vac 變數初始化設定*/
    Vac_Bwron_in_Cnt = 0;
    Vac_peak_temp = 0;
    Vac_peak = 0;
}
