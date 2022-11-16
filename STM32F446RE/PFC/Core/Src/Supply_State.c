
#include "systemsetting.h"
struct PFC_VARIABLES PFC_Variables;
/*Variable*/
uint8_t Bwrom_IN_Flag;
uint16_t Vac_Bwron_in_Cnt;
uint16_t Vac_peak;
uint16_t Vac_peak_temp;
/*正半周*/
inline void clear_positive_accumulators(void)
{
    PFC_Variables.positive_cycle_counter = 0;
    PFC_Variables.positive_vin_squared_accumulate = 0;
}

inline void store_positive_cycle_values(void)
{
    PFC_Variables.vin_squared_average = PFC_Variables.positive_vin_squared_accumulate / PFC_Variables.positive_cycle_counter;

    PFC_Variables.vin_squared_for_ac_drop = PFC_Variables.vin_squared_average;
}

inline void accumulate_positive_cycle_values(void)
{
    PFC_Variables.positive_vin_squared_accumulate = PFC_Variables.vin_squared + PFC_Variables.positive_vin_squared_accumulate;
}

/*負半周*/
inline void clear_negative_accumulators(void)
{
    PFC_Variables.negative_cycle_counter = 0;
    PFC_Variables.negative_vin_squared_accumulate = 0;
}

inline void store_negative_cycle_values(void)
{
    PFC_Variables.vin_squared_average = PFC_Variables.negative_vin_squared_accumulate / PFC_Variables.negative_cycle_counter;

    PFC_Variables.vin_squared_for_ac_drop = PFC_Variables.vin_squared_average;
}

inline void accumulate_negative_cycle_values(void)
{
    PFC_Variables.negative_vin_squared_accumulate = PFC_Variables.vin_squared + PFC_Variables.negative_vin_squared_accumulate;
}

/*AC 整流判斷正負半周*/
inline void rectify_vac(void)
{
    /*AC 正負半周判斷*/
    if (PFC_Variables.adc_raw[AC_L_CHANNEL] > PFC_Variables.adc_raw[AC_N_CHANNEL])
    {
        PFC_Variables.vin_raw = PFC_Variables.adc_raw[AC_L_CHANNEL] - PFC_Variables.adc_raw[AC_N_CHANNEL];
        PFC_Variables.AC_STATE = Postive;
    }
    else
    {
        PFC_Variables.vin_raw = PFC_Variables.adc_raw[AC_N_CHANNEL] - PFC_Variables.adc_raw[AC_L_CHANNEL];
        PFC_Variables.AC_STATE = Negative;
    }
    /*移動平均計算AC*/
    PFC_Variables.vin_sum = PFC_Variables.vin_raw + PFC_Variables.vin_sum - (PFC_Variables.vin_sum >> 2);
    PFC_Variables.vin_filtered = PFC_Variables.vin_sum >> 2;
    /*Vac PEAK 計算*/
    if (PFC_Variables.vin_filtered > Vac_peak_temp)
    {
        Vac_peak_temp = PFC_Variables.vin_filtered;
    }
}

/*半周計算*/
inline void half_cycle_processing(void)
{
    /*正半周*/
    if (PFC_Variables.AC_STATE == Postive)
    {
        PFC_Variables.positive_cycle_counter++;
        if ((PFC_Variables.positive_cycle_counter == 4) && (PFC_Variables.negative_cycle_counter >= 16))
        {
            store_negative_cycle_values();
            clear_negative_accumulators();
            Vac_peak=Vac_peak_temp;
            Vac_peak_temp=0;
        }
        else if (PFC_Variables.positive_cycle_counter > 4)
        {
            clear_negative_accumulators(); // to deal with possible negative glitches
            if (PFC_Variables.positive_cycle_counter >= 127)
            {
                store_positive_cycle_values();
                clear_positive_accumulators();
            }
        }
        accumulate_positive_cycle_values();
    }
    /*負半周*/
    else
    {
        PFC_Variables.negative_cycle_counter++;
        if ((PFC_Variables.negative_cycle_counter == 4) && (PFC_Variables.positive_cycle_counter >= 16))
        {
            store_positive_cycle_values();
            clear_positive_accumulators();
            Vac_peak=Vac_peak_temp;
            Vac_peak_temp=0;
        }
        else if (PFC_Variables.negative_cycle_counter > 4)
        {
            clear_negative_accumulators(); // to deal with possible negative glitches
            if (PFC_Variables.negative_cycle_counter >= 127)
            {
                store_negative_cycle_values();
                clear_negative_accumulators();
            }
        }
        accumulate_negative_cycle_values();
    }
}

/*Singal Voltage loop*/
// inline int32 proportional_integral(int32 error)
// {
//     /*計算穩太誤差*/
//     /*確認DUTY Limit*/
//     /*跟新DUTY*/

// }

/*2P2Z控制器*/

// PFC_VARIABLES PFC_Variables;
/*Suplly STATE Function*/

// 確認是否進入Bwron in 點
inline void idle_state_handler(void)
{
    if (VBulk > 1.5)
    // if (analog_result_volt > Bwron_in_point)
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
    if (VBulk > 1.5)
    // if (analog_result_volt > Bwron_in_point)
    {
        Vac_Bwron_in_Cnt++;
        if (Vac_Bwron_in_Cnt > 10)
        {
            Bwrom_IN_Flag = True;
        }
    }
    else
    {
        Vac_Bwron_in_Cnt = 0;
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
    /*偵測ADC是否為該值並發送PGI*/
    if (VBulk > 1.5)
    // if (analog_result_volt > Bwron_in_point)
    {
        HAL_GPIO_WritePin(PGI_GPIO_PORT, Power_GOOD_PIN, GPIO_PIN_SET); // SEND PGI
        PFC_Variables.supply_state = STATE_PFC_ON;                      // PFC normal mode
    }
}
// 常態模式下啟動PFC
/* PFC 常態下*/
inline void pfc_on_state_handler(void)
{
    /*Vac 偵測  Bwron out*/
    if (VBulk < 1.0)
    // if (analog_result_volt < Bwron_in_point)
    {
        turn_off_pfc();                                                   /*STOP PFC*/
        HAL_GPIO_WritePin(PGI_GPIO_PORT, Power_GOOD_PIN, GPIO_PIN_RESET); // STOP PGI
        PFC_Variables.supply_state = STATE_IDLE;                          // PFC Back IDLE WAIT FOR AC
    }

    /**/
}
//

// 異常問題關閉PFC
inline void pfc_shut_down_state_handler(void)
{
    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
    HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_1);
}

//
inline void pfc_hiccup_state_handler(void)
{
}

// PWM enable
void turn_on_pfc(void)
{
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);    // Enable high side
    HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1); // Enable low side
}

// pwm disable
void turn_off_pfc(void)
{
    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
    HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_1);
}

/*********************PFC supply state*********************/
inline void supply_state_handler(void)
{

    switch (PFC_Variables.supply_state)
    {
    case STATE_IDLE:
        // Uart_sendstring("PFC is in idle mode", device_uart);
        idle_state_handler();
        break;

    case STATE_RELAY_BOUNCE:
        // Uart_sendstring("PFC is in relay bounce mode", device_uart);
        relay_bounce_state_handler();
        break;

    case STATE_RAMP_UP:
        // Uart_sendstring("PFC is in Ramp up mode", device_uart);
        ramp_up_state_handler();
        break;

    case STATE_PFC_ON:
        // Uart_sendstring("PFC is in PFC ON mode", device_uart);
        pfc_on_state_handler();
        break;

    case STATE_PFC_HICCUP:
        // Uart_sendstring("PFC is in hiccup mode", device_uart);
        pfc_hiccup_state_handler();
        break;

    case STATE_PFC_SHUT_DOWN:
        // Uart_sendstring("PFC is in shunt down mode", device_uart);
        pfc_shut_down_state_handler();
        break;

    default:
        break;
    }
}

/*********************PFC TASK STATE*********************/
void PFC_TASK_STATE(void)
{
    // switch (PFC_Variables.task_state)
    // {
    // case I_STATE_1: // 電壓環

    //     PFC_Variables.task_state = I_STATE_2;
    //     break;

    // case I_STATE_2: // AC 計算

    //     PFC_Variables.task_state = I_STATE_3;
    //     break;

    // case I_STATE_3: // AC 跌落

    //     PFC_Variables.task_state = I_STATE_4;
    //     break;

    // case I_STATE_4: // EMI dithering

    //     PFC_Variables.task_state = I_STATE_5;
    //     break;

    // case I_STATE_5: // PFC狀態機
    supply_state_handler();
    //     PFC_Variables.task_state = I_STATE_1;
    //     break;

    // default: // 錯誤則從STATE1開始
    //     PFC_Variables.task_state = I_STATE_1;
    //     break;
    // }
}
