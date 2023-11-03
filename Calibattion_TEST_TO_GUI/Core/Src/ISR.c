#include "main.h"

/*ISR call Back Handler*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim==&htim10)
    {
        //ADC Scan
        Multi_ADC_Sample();
       
        
    }
}







