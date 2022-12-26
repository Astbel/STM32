
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
void ADC_Select_CH0(void);
void ADC_Select_CH1(void);
void ADC_Select_CH2(void);
void ADC_Select_CH3(void);
/*ADC Polling Sample*/
void ADC_Sample(void)
{
    /*Check Task */

    HAL_ADC_Start(&hadc1);

    if (HAL_ADC_PollForConversion(&hadc1, 1000) == HAL_OK)
    {

        /*實測量起來有多個OFFSET存在目前測試為0.26V*/
        // VBulk = (((float)analog_result_volt)*3/(float)4095)-0.23;
        // VBulk = (((float)analog_result_volt) * 3.3 / (float)4095);

        // for (Sample_cnt = 0; Sample_cnt < ADC_Sample_Rate; Sample_cnt++)
        // {
        //     ADC_Result[Sample_cnt] = ADC1->DR;
        // }
        // Vmax = Vmin = ADC_Result[0];

        // for (Sample_cnt = 0; Sample_cnt < ADC_Sample_Rate; Sample_cnt++)
        // {
        //     if (Vmin > ADC_Result[Sample_cnt + 1])   //find min
        //         Vmin = ADC_Result[Sample_cnt + 1];

        //     if (Vmax < ADC_Result[Sample_cnt + 1])   //find max
        //         Vmax = ADC_Result[Sample_cnt + 1];
        // }

        // for ( Sample_cnt = 0; Sample_cnt < ADC_Sample_Rate; Sample_cnt++)
        // {
        //     Vac_temp +=ADC_Result[Sample_cnt];
        // }
        // Vac_peak =(Vac_temp-Vmax-Vmin)/(ADC_Sample_Rate-2);

        // analog_result_volt = HAL_ADC_GetValue(&hadc1);

        // Vac_filter = analog_result_volt + Vac_filter - (Vac_filter >> 2);

        // Vac_PP = Vac_filter >> 2;

        // if (Vac_filter > Vac_PP)
        // {
        //     Vac_PP = Vac_filter;
        // }
        /*換相後拿PEAK值在清空比較*/
        if (PFC_Variables.AC_STATE == Postive)
        {
            Vac_peak = Vac_peak_temp;
            Vac_peak_temp = 0;
        }

        /*比較每次取樣值去找AC PEAK點 */
        // if (analog_result_volt > Vac_temp)
        // {
        //     Vac_temp = analog_result_volt;
        // }
        // /*跟新值假設計數發現新的Vac_PP比較小當作是Vac下降*/
        // if (Vac_temp > Vac_PP)
        // {
        //     Vac_PP = Vac_temp+ADC_DC_OffSet; // 需要補上DC offset
        //     Vac_temp = 0;
        //     Sample_cnt = 0;
        // }
        // else if ((Vac_PP > Vac_temp) && (Sample_cnt == 0)) // 當ac條壓後比較之前的Vpeak開始計數換檔
        // {
        //     Sample_cnt++;
        //     if (Sample_cnt == 100)
        //     {
        //         Sample_cnt = 101;
        //     }
        // }
        // else if (Sample_cnt == 101)
        // {
        //     Vac_PP = Vac_temp+ADC_DC_OffSet; // 需要補上DC offset
        // }

        // /*Uart send back value*/
        // real_ac = (float)((Vac_PP * 3.3) / 4096); /*Peak Value here*/

        /*Moving avg*/
        // test_ac_sample = analog_result_volt + test_ac_sample - (test_ac_sample >> 2);
        // ac_sum = test_ac_sample >> 2;

        // if (ac_sum > Vac_temp) // 找peak 點
        // {
        //     Vac_temp = ac_sum;
        // }

        // if (Vac_temp > Vac_PP)
        // {
        //     Vac_PP = Vac_temp <<1;
        //     Vac_temp = 0;
        // }
        // real_ac = (double)((Vac_PP * 3.0) / 4096);

        // analog_result_volt = VBulk;
    }
    HAL_ADC_Stop(&hadc1);

    HAL_GPIO_WritePin(PGI_GPIO_PORT, Power_GOOD_PIN, GPIO_PIN_RESET);
}
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
//Ind current
void ADC_Select_CH3(void)
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
    ADC_Select_CH3();
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, 1000);
    PFC_Variables.adc_raw[3] = HAL_ADC_GetValue(&hadc1);
    // VBulk = (float)((PFC_Variables.adc_raw[2] * 400) / 4096);
    // VBulk = (float)((PFC_Variables.adc_raw[2] * 405) / 2240);
    HAL_ADC_Stop(&hadc1);
}

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
