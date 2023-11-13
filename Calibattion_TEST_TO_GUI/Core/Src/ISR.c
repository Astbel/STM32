#include "main.h"

/*ISR call Back Handler*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim==&htim10)
    {
        //ADC Scan
        // Multi_ADC_Sample();
        
        // C# LOG獲得命令
        // Get_Command_From_C_shrap();

    }
}







