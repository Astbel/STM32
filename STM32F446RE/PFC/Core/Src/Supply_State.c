
#include "systemsetting.h"
struct PFC_VARIABLES PFC_Variables;
struct PID PID;
/*Variable*/
uint8_t Bwrom_IN_Flag;
uint16_t Vac_Bwron_in_Cnt;
uint16_t Vac_peak;
uint16_t Vac_peak_temp;
int32_t steady_state_err;
int32_t Voltage_Kp;
int32_t Voltage_Ki;
uint32_t Bulk_Volt_Target;
uint32_t Power_good_PFC_Bulk_Target;
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
    // if (PFC_Variables.adc_raw[AC_L_CHANNEL] > PFC_Variables.adc_raw[AC_N_CHANNEL])
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
    /*每次都保留最後四筆資料*/
    /*要取當前值所以總和/比數*/
    PFC_Variables.vin_sum = PFC_Variables.vin_raw + PFC_Variables.vin_sum - (PFC_Variables.vin_sum >> 10);
    PFC_Variables.vin_filtered = PFC_Variables.vin_sum >> 10;
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
        // PFC_Variables.positive_cycle_counter++;
        // if ((PFC_Variables.positive_cycle_counter == 4) && (PFC_Variables.negative_cycle_counter >= 16))
        // {
        // store_negative_cycle_values();
        // clear_negative_accumulators();
        Vac_peak = Vac_peak_temp;
        Vac_peak_temp = 0;
        // }
        // else if (PFC_Variables.positive_cycle_counter > 4)
        // {
        //     clear_negative_accumulators(); // to deal with possible negative glitches
        //     if (PFC_Variables.positive_cycle_counter >= 127)
        //     {
        //         store_positive_cycle_values();
        //         clear_positive_accumulators();
        //     }
        // }
        // accumulate_positive_cycle_values();
    }
    /*負半周*/
    else
    {
        // PFC_Variables.negative_cycle_counter++;
        // if ((PFC_Variables.negative_cycle_counter == 4) && (PFC_Variables.positive_cycle_counter >= 16))
        // {
        //     store_positive_cycle_values();
        //     clear_positive_accumulators();
        Vac_peak = Vac_peak_temp;
        Vac_peak_temp = 0;
        //     }
        //     else if (PFC_Variables.negative_cycle_counter > 4)
        //     {
        //         clear_negative_accumulators(); // to deal with possible negative glitches
        //         if (PFC_Variables.negative_cycle_counter >= 127)
        //         {
        //             store_negative_cycle_values();
        //             clear_negative_accumulators();
        //         }
        //     }
        //     accumulate_negative_cycle_values();
    }
    real_ac = (float)(Vac_peak * 264) / 1600;
}

/*Singal Voltage loop*/
/*
 *  積分飽和消除方法遇限消除法
 *  正飽和 只看積分負向誤差
 *  負飽和 只看積分正向誤差
 */
inline int32_t proportional_integral(int32_t error)
{

    /*Test for PI PWM need to be disable pwm*/
    // turn_on_pfc();     測試用

    int16_t output_duty;
    steady_state_err = error;

    /*穩態誤差在範圍內*/
    /*Kp*/
    Voltage_Kp = PID.kp * steady_state_err;
    /*Ki*/
    Voltage_Ki += (PID.ki * T * steady_state_err);

    // 限制積分值必面積分飽和,遇限消除法
    if (Voltage_Ki > I_MAX)
    {
        Voltage_Ki = I_MAX;
    }
    else if (Voltage_Ki < I_MIN)
    {
        Voltage_Ki = I_MIN;
    }

    output_duty = (Voltage_Kp + Voltage_Ki) >> 12; // Q27>>12轉至籌Q15格式轉換

    // /*確認DUTY Limit*/
    /*Max  68%*/
    if (output_duty > MAX_DUTY)
    {
        output_duty = MAX_DUTY;
    }
    /*Min 10%*/
    if (output_duty < MIN_DUTY)
    {
        output_duty = MIN_DUTY;
    }

    /*跟新DUTY*/
    TIM1->CCR1 = output_duty;
    // TIM2->CCR3 = output_duty; // Phase A
    // TIM3->CCR3 = output_duty; // Phase B
}

/*2P2Z控制器*/

// PFC_VARIABLES PFC_Variables;
/*Suplly STATE Function*/

// 確認是否進入Bwron in 點
inline void idle_state_handler(void)
{
    // if (VBulk > 1.5)
    if (Vac_peak > Bwron_in_point)
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
    // if (VBulk > 1.5)
    if (Vac_peak > Bwron_in_point)
    {
        //check AC and set bulk target
        Vac_Check_Bulk_Cap_Target();
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
    /*等待PFC電壓升到對應Vac該值才開啟PFC*/
    if ((Bwrom_IN_Flag == True)&&(PFC_Variables.adc_raw[VBUS_CHANNEL] > Bulk_Volt_Target))
    {
        Vac_Bwron_in_Cnt = 0;
        Bwrom_IN_Flag = False;
         turn_on_pfc();
        PFC_Variables.supply_state = STATE_RAMP_UP;
    }
}
// VBULK 爬升 送PGI致2次側
inline void ramp_up_state_handler(void)
{
    /*PFC soft start ramp up need to write here*/
    /*Vbulk OVP*/
    if (PFC_Variables.adc_raw[VBUS_CHANNEL] > OVer_Voltage_VBULK)
    {
        turn_off_pfc();
        PFC_Variables.supply_state = STATE_PFC_SHUT_DOWN;
    }
    /*等待PFC電壓升到對應Vac該值才開啟PFC*/
     if (PFC_Variables.adc_raw[VBUS_CHANNEL] > Power_good_PFC_Bulk_Target)
    {
        GPIOA->BSRR = 0x04;
        // HAL_GPIO_WritePin(PGI_GPIO_PORT, Power_GOOD_PIN, GPIO_PIN_SET); // SEND PGI
        PFC_Variables.supply_state = STATE_PFC_ON; // PFC normal mode
    }
}
// 常態模式下啟動PFC
/* PFC 常態下*/
inline void pfc_on_state_handler(void)
{
    /*Vbulk OVP*/
    if (PFC_Variables.adc_raw[VBUS_CHANNEL] > OVer_Voltage_VBULK)
    {
        turn_off_pfc();
        PFC_Variables.supply_state = STATE_PFC_SHUT_DOWN;
        // inital the variable
    }

    /*Vac 偵測  Bwron out*/
    // if (VBulk < 1.0)
    if (Vac_peak < Bwron_out_point)
    {
        turn_off_pfc();
        // HAL_GPIO_WritePin(PGI_GPIO_PORT, Power_GOOD_PIN, GPIO_PIN_RESET); // STOP PGI
        PFC_Variables.supply_state = STATE_IDLE; // PFC Back IDLE WAIT FOR AC
    }

    /**/
}
//

// 異常問題關閉PFC
inline void pfc_shut_down_state_handler(void)
{
    /*LOCK DOWN*/
    turn_off_pfc();
    // Initail_Variable();
}

//
inline void pfc_hiccup_state_handler(void)
{
}

// PWM enable
void turn_on_pfc(void)
{
    // HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);    // Enable high side
    // HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1); // Enable low side
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1); // PWM Master ClK
    // HAL_TIM_OC_Start(&htim1, TIM_CHANNEL_2);
    // HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3); // Phase A
    // HAL_TIM_OC_Start(&htim2, TIM_CHANNEL_4);
    // HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3); // Phase B
}

// pwm disable
void turn_off_pfc(void)
{
    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
    // HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_1);
    //     HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
    //     HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_3); // Pwm output channel Phase A
    //     HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_3); // Pwm output channel Phase B
    //     HAL_TIM_OC_Stop(&htim1, TIM_CHANNEL_2);  // Trigger  Master for Phase A
    //     HAL_TIM_OC_Stop(&htim2, TIM_CHANNEL_4);  // Trigger Master for Phase B
}
/*********************Vac_Bulk******************************/
/*
* Bulk_Volt_Targe              AC開啟後等待VBUS升到對應值
*  Power_good_PFC_Bulk_Target  PGI送的電壓目標
*/
void Vac_Check_Bulk_Cap_Target(void)
{
    if (real_ac >= 240) // 264
    {
        Bulk_Volt_Target = 0x080F;
        Power_good_PFC_Bulk_Target =Vref;
    }
    else if ((real_ac >= 185) && (real_ac < 240)) // 230
    {
        Bulk_Volt_Target = 0x0796;
       Power_good_PFC_Bulk_Target =Vref;
    }
    else if ((real_ac > 105) && (real_ac < 185)) // 115
    {
        Bulk_Volt_Target = 0x0380;
        Power_good_PFC_Bulk_Target =Vref;
    }
    else // 90
    {
        Bulk_Volt_Target = 0x02BE;
        Power_good_PFC_Bulk_Target =Vref;
    }
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
    switch (PFC_Variables.task_state)
    {
    case I_STATE_1: // 電壓環
        proportional_integral(Vref - PFC_Variables.adc_raw[VBUS_CHANNEL]);
        PFC_Variables.task_state = I_STATE_2;
        break;

    case I_STATE_2: // AC 計算
        half_cycle_processing();
        // PFC_Variables.task_state = I_STATE_3;
        PFC_Variables.task_state = I_STATE_5;
        break;

        // case I_STATE_3: // AC 跌落

        //     PFC_Variables.task_state = I_STATE_4;
        //     break;

        // case I_STATE_4: // EMI dithering

        //     PFC_Variables.task_state = I_STATE_5;
        //     break;

    case I_STATE_5: // PFC狀態機
        supply_state_handler();
        PFC_Variables.task_state = I_STATE_1;
        break;

    default: // 錯誤則從STATE1開始
        PFC_Variables.task_state = I_STATE_1;
        break;
    }
}

/*ISR call  back Function*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    // Check which version of the timer triggered this callback and toggle LED
    if (htim == &htim10)
    {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5); /*觀測點確認ISR執行*/

        Multi_ADC_Sample();

        rectify_vac();

        PFC_TASK_STATE();
    }
}
