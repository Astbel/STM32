#include "main.h"
void UART_TEST_SEND(void);
/*ISR call Back Handler*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim == &htim10)
    {
        // ADC Scan 
        //  Multi_ADC_Sample();
        HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
        // C# LOG獲得命令
        // Get_Command_From_C_shrap();
        
        /*Sprintf is fine */
        // UART_TEST_SEND();
    }
}

// Test sprintf
void UART_TEST_SEND(void)
{
    char buffer[50];
    int volt_test = rand() % 100;
    int curr_test = rand() % 100;
    sprintf(buffer, "volt is %d, curr is %d", volt_test, curr_test);
    Uart_sendstring(buffer, pc_uart);
}
