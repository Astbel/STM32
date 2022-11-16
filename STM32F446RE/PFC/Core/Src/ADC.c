
#include "systemsetting.h"
/*Variable*/
uint16_t analog_result_volt;
uint16_t ADC_Result[3];
float VBulk;
 float adc_sample1; 
 float adc_sample2;
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
    HAL_ADC_Start(&hadc1);

    if (HAL_ADC_PollForConversion(&hadc1, 1000) == HAL_OK)
    {
        analog_result_volt = HAL_ADC_GetValue(&hadc1);
        /*實測量起來有多個OFFSET存在目前測試為0.26V*/
        // VBulk = (((float)analog_result_volt)*3/(float)4095)-0.23;
        VBulk = (((float)analog_result_volt) * 3.3 / (float)4095);
        analog_result_volt = VBulk;
    }
    HAL_ADC_Stop(&hadc1);
    // analog_result_volt =HAL_ADC_GetValue(&hadc1);
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
    ADC_Result[0] = HAL_ADC_GetValue(&hadc1);
    adc_sample1=(((float)ADC_Result[0]) * 3.3 / (float)4095);
    HAL_ADC_Stop(&hadc1);

    ADC_Select_CH1();
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, 1000);
    ADC_Result[1] = HAL_ADC_GetValue(&hadc1);
    adc_sample2=(((float)ADC_Result[1]) * 3.3 / (float)4095);
    HAL_ADC_Stop(&hadc1);
}

/*Average value of the Result*/
/*之後要測試ADC 掃AC 要抓AC之後座移動平均*/
