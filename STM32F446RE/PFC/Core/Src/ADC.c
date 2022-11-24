
#include "systemsetting.h"
/*Variable*/
uint16_t analog_result_volt;
uint16_t ADC_Result[64];
float VBulk;
float adc_sample1;
float adc_sample2;
uint32_t test_ac_sample;
uint32_t ac_sum;
float real_ac;
uint16_t Sample_cnt;
uint16_t Vac_temp;
uint16_t Vac_PP;
/*ADC 取樣 5 Channel*/
/*
1. VBulk
2. Phase_A_IL
3. Phase_B_IL
4. Vac_L
5. Vac_N
*/

/*Function define*/
void ADC_Select_CH0(void);
void ADC_Select_CH1(void);

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

        analog_result_volt = HAL_ADC_GetValue(&hadc1);

        /*比較每次取樣值去找AC PEAK點 */
        if (analog_result_volt > Vac_temp)
        {
            Vac_temp = analog_result_volt;
        }
        /*跟新值假設計數發現新的Vac_PP比較小當作是Vac下降*/
        if (Vac_temp > Vac_PP)
        {
            Vac_PP = Vac_temp+ADC_DC_OffSet; // 需要補上DC offset
            Vac_temp = 0;
            Sample_cnt = 0;
        }
        else if ((Vac_PP > Vac_temp) && (Sample_cnt == 0)) // 當ac條壓後比較之前的Vpeak開始計數換檔
        {
            Sample_cnt++;
            if (Sample_cnt == 100)
            {
                Sample_cnt = 101;
            }
        }
        else if (Sample_cnt == 101)
        {
            Vac_PP = Vac_temp+ADC_DC_OffSet; // 需要補上DC offset
        }

        /*Uart send back value*/
        real_ac = (float)((Vac_PP * 3.3) / 4096); /*Peak Value here*/

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

void ADC_Select_CH1(void)
{
    ADC_ChannelConfTypeDef sConfig = {0};
    /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
     */
    sConfig.Channel = ADC_CHANNEL_1;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_84CYCLES;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }
}
/*Task for sample multi data*/
void Multi_ADC_Sample(void)
{
    ADC_Select_CH0();
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, 1000);
    PFC_Variables.adc_raw[AC_N_CHANNEL] = HAL_ADC_GetValue(&hadc1);
    adc_sample1 = (((float)PFC_Variables.adc_raw[AC_N_CHANNEL]) * 3.3 / (float)4095);
    HAL_ADC_Stop(&hadc1);

    ADC_Select_CH1();
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, 1000);
    PFC_Variables.adc_raw[AC_L_CHANNEL] = HAL_ADC_GetValue(&hadc1);
    adc_sample2 = (((float)PFC_Variables.adc_raw[AC_L_CHANNEL]) * 3.3 / (float)4095);
    HAL_ADC_Stop(&hadc1);
}

/*Average value of the Result*/
/*之後要測試ADC 掃AC 要抓AC之後座移動平均*/
