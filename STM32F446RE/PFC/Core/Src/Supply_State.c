
#include "systemsetting.h"
struct PFC_VARIABLES PFC_Variables;
/*Variable*/
uint8_t Bwrom_IN_Flag;
uint16_t Vac_Bwron_in_Cnt;

// PFC_VARIABLES PFC_Variables;
/*Suplly STATE Function*/

// 確認是否進入Bwron in 點
inline void idle_state_handler(void)
{
    if (analog_result_volt > Bwron_in_point)
    {
        Bwrom_IN_Flag++;
    }
    else
    {
        Bwrom_IN_Flag = False;
    }

    if (Bwrom_IN_Flag == True)
    {
        Bwrom_IN_Flag = False;
        PFC_Variables.supply_state = STATE_RELAY_BOUNCE;
    }
}
//
inline void relay_bounce_state_handler(void)
{
    if (analog_result_volt > Bwron_in_point)
    {
        Bwrom_IN_Flag++;
    }
    else
    {
        Bwrom_IN_Flag = False;
    }

    if (Bwrom_IN_Flag == True)
    {
        Bwrom_IN_Flag = False;
        turn_on_pfc();
        PFC_Variables.supply_state = STATE_RAMP_UP;
    }
}
// VBULK 爬升 送PGI致2次側
inline void ramp_up_state_handler(void)
{
}
// 常態模式下啟動PFC
/* PFC 常態下*/
inline void pfc_on_state_handler(void)
{
    /*Vac 偵測  Bwron out*/

    /**/
}
//

// 異常問題關閉PFC
inline void pfc_shut_down_state_handler(void)
{
   HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_1);
   HAL_TIMEx_PWMN_Stop(&htim1,TIM_CHANNEL_1);
}

//
inline void pfc_hiccup_state_handler(void)
{

}

// PWM enable
void turn_on_pfc(void)
{
   HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
   HAL_TIMEx_PWMN_Start(&htim1,TIM_CHANNEL_1);
}

// pwm disable
void turn_off_pfc(void)
{
    HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_1);
    HAL_TIMEx_PWMN_Stop(&htim1,TIM_CHANNEL_1);
}

/*********************PFC supply state*********************/
inline void supply_state_handler(void)
{

    switch (PFC_Variables.supply_state)
    {
        case STATE_IDLE:
             Uart_sendstring("PFC is in idle mode",device_uart);
             idle_state_handler();
        break;

        case STATE_RELAY_BOUNCE:
           Uart_sendstring("PFC is in relay bounce mode",device_uart);
           relay_bounce_state_handler();
        break;

        case STATE_RAMP_UP:
           Uart_sendstring("PFC is in Ramp up mode",device_uart);
           ramp_up_state_handler();
        break;

        case STATE_PFC_ON:
            Uart_sendstring("PFC is in PFC ON mode",device_uart);
            pfc_on_state_handler();
        break;

        case STATE_PFC_HICCUP:
             Uart_sendstring("PFC is in hiccup mode",device_uart);
             pfc_hiccup_state_handler();
        break;

        case STATE_PFC_SHUT_DOWN:
             Uart_sendstring("PFC is in shunt down mode",device_uart);
             pfc_shut_down_state_handler();
        break;


       default:
        break;
    }
}

/*********************PFC TASK STATE*********************/
void PFC_TASK_STATE(void)
{
    switch (PFC_Variables.task_state)
    {
    case I_STATE_1:   //電壓環
        
        PFC_Variables.task_state=I_STATE_2;
        break;
    
    case I_STATE_2:  //AC 計算
        
        PFC_Variables.task_state=I_STATE_3;
        break;

    case I_STATE_3:  //AC 跌落
       
        PFC_Variables.task_state=I_STATE_4;
        break;

    case I_STATE_4:  //EMI dithering
       
       PFC_Variables.task_state=I_STATE_5;
        break;

    case I_STATE_5:  //PFC狀態機
       supply_state_handler();
       PFC_Variables.task_state=I_STATE_1;
        break;

    
    default:  //錯誤則從STATE1開始
        PFC_Variables.task_state =I_STATE_1; 
        break;
    }
}






