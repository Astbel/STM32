
#include "systemsetting.h"
/*Variable*/
uint16_t analog_result_volt;
float VBulk;

/*ADC Polling Sample*/
void ADC_Sample(void)
{
    HAL_ADC_Start(&hadc1);
    
    if (HAL_ADC_PollForConversion(&hadc1,1000)==HAL_OK)
    {
        analog_result_volt =HAL_ADC_GetValue(&hadc1);
        /*實測量起來有多個OFFSET存在目前測試為0.26V*/
        VBulk = (((float)analog_result_volt)*3/(float)4095)-0.23;
        analog_result_volt =VBulk;
    }
    HAL_ADC_Stop(&hadc1);
    // analog_result_volt =HAL_ADC_GetValue(&hadc1);


}
/*Average value of the Result*/
/*之後要測試ADC 掃AC 要抓AC之後座移動平均*/

