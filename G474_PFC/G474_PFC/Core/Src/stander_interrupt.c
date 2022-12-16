#include "systemsetting.h"


/*System PFC ISR Hander Loop 0.18ms perious a time*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  // Check which version of the timer triggered this callback and toggle LED
  if (htim == &htim16 )
  {
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);  /*觀測點確認ISR執行*/

    HAL_ADCEx_Calibration_Start(&hadc1,ADC_SINGLE_ENDED );
    
    Multi_Sweep_ADC_Sample(); /* ADC 取樣*/

    rectify_vac();      /*AC整流找PEAK 峰值*/

    PFC_TASK_STATE();  /*PFC狀態切換*/
  }
}


