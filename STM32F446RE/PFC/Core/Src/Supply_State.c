
#include "systemsetting.h"
struct Volt_Controller_3p3z Volt_comtroller_3p3z;
struct PFC_VARIABLES PFC_Variables;
struct AC_Drop AC_Drop;
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

/**
 * @brief sweeping AC
 * Task 1 Find Peak Vac
 * Task 2 Find Zero Cross
 * Task 3 summ Rect AC of the Briged
 */
/*Sweeping AC L and N*/
inline void
AC_Sweeping_Method(void)
{
    // Intergal Rect AC
    // Line is bigger than Neturl
    if (PFC_Variables.adc_raw[AC_L_CHANNEL] > PFC_Variables.adc_raw[AC_N_CHANNEL])
    {
        ac_rect_sum += PFC_Variables.adc_raw[AC_L_CHANNEL];
        if (Vac_peak_temp < PFC_Variables.adc_raw[AC_L_CHANNEL])
            Vac_peak_temp = PFC_Variables.adc_raw[AC_L_CHANNEL];
    }
    // Neturl is bigger than Line
    else
    {
        ac_rect_sum += PFC_Variables.adc_raw[AC_N_CHANNEL];
        if (Vac_peak_temp < PFC_Variables.adc_raw[AC_N_CHANNEL])
            Vac_peak_temp = PFC_Variables.adc_raw[AC_N_CHANNEL];
    }

    //  Find Zero Cross
    while ((PFC_Variables.adc_raw[AC_L_CHANNEL] <= Zero_Cross_Point) || (PFC_Variables.adc_raw[AC_N_CHANNEL] <= Zero_Cross_Point))
    {
        // here is find a Zero Cross do method here
    }
    // Updata Peak Vac  reset Peak Vac temp
    if (Vac_peak < Vac_peak_temp)
    {
        Vac_peak = Vac_peak_temp;
        Vac_peak_temp = Reset;
    }
}
/**
 * @brief Volt_Loop_Controller_Coeffiect_Init
 *
 * This method is for inital the  Volt loop 3p3z coeffiect
 * Here need too Normalized the Coeffeiect
 */
inline void Volt_Loop_Controller_Coeffiect_Init(void)
{
    // Coeffiecient B Input
    Volt_comtroller_3p3z.B3 = 0x84CC;
    Volt_comtroller_3p3z.B2 = 0x04C8;
    Volt_comtroller_3p3z.B1 = 0x7FFC;
    Volt_comtroller_3p3z.B0 = 0xFB38;
    // Coeffiecient A Output
    Volt_comtroller_3p3z.A3 = 0x042F;
    Volt_comtroller_3p3z.A2 = 0x30AB;
    Volt_comtroller_3p3z.A1 = 0x4825;
}

/**
 * @brief  Volt_comtroller_3p3z  Type 3 of the Voltage comapstion
 *
 *
 */
inline void Volt_Loop_Controller(void)
{
    // Counter for the Differential equation
    static uint8_t cnt;

    // Get the input Signal
    Volt_comtroller_3p3z.Xn[0] = PFC_Variables.VBulk_command - PFC_Variables.adc_raw[VBUS_CHANNEL];
    // Differenationl the equation
    Volt_comtroller_3p3z.Yn[0] = Volt_comtroller_3p3z.B3 * Volt_comtroller_3p3z.Xn[3] + Volt_comtroller_3p3z.B2 * Volt_comtroller_3p3z.Xn[2] + Volt_comtroller_3p3z.B1 * Volt_comtroller_3p3z.Xn[1] + Volt_comtroller_3p3z.B0 * Volt_comtroller_3p3z.Xn[0] +
                                 Volt_comtroller_3p3z.A3 * Volt_comtroller_3p3z.Yn[3] + Volt_comtroller_3p3z.A2 * Volt_comtroller_3p3z.Yn[2] + Volt_comtroller_3p3z.A1 * Volt_comtroller_3p3z.Yn[1];
    // Updata the data
    for (cnt = Sample; cnt > 0; cnt--)
    {
        Volt_comtroller_3p3z.Xn[cnt] = Volt_comtroller_3p3z.Xn[cnt - 1];
        Volt_comtroller_3p3z.Yn[cnt] = Volt_comtroller_3p3z.Yn[cnt - 1];
    }
    Volt_comtroller_3p3z.Voltage_Loop_Out = Volt_comtroller_3p3z.Yn[0];
    // Limit the Max and Min duty
    if (Volt_comtroller_3p3z.Voltage_Loop_Out > MAX_DUTY)
        Volt_comtroller_3p3z.Voltage_Loop_Out = MAX_DUTY;
    else if (Volt_comtroller_3p3z.Voltage_Loop_Out < MIN_DUTY)
        Volt_comtroller_3p3z.Voltage_Loop_Out = MIN_DUTY;

    // Send Duty Register
    TIM1->CCR1 = Volt_comtroller_3p3z.Voltage_Loop_Out;
}
/**
 * @brief AC Drop
 *        AC disapperted method shut dwon pfc countting the timing of the timing
 */
inline void AC_Drop_Method(void)
{
    //AC 整流判定低於特定Vac時開始計數
    if (ac_rect_sum < AC_Drop.ac_rect_drop_target)
        AC_Drop.ac_drop_cnt++;
    
    //當Vac不見一段時間後
    while (AC_Drop.ac_drop_cnt > AC_Drop_CNT_ShutDown)
    {
        //Consider limit the duty cycle
        turn_off_pfc();
        PFC_Variables.supply_state=STATE_IDLE;
    }
    
    //Vac在一定時間賦歸
    if(ac_rect_sum > AC_Drop.ac_rect_drop_target)
     {
        AC_Drop.ac_drop_cnt=RESET;
     }   

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
// 確認是否進入Bwron in 點
inline void idle_state_handler(void)
{
    //RESET Value here
    AC_Drop.ac_drop_cnt=Reset;

    if (Vac_peak > Bwron_in_point)
    {
        PFC_Variables.supply_state = STATE_RELAY_BOUNCE;
    }
}
//
inline void relay_bounce_state_handler(void)
{
    // here need fix chack ac and check the bulk volt if it boost to the right sqrt2 of the Vac
    if (PFC_Variables.adc_raw[VBUS_CHANNEL] > Bwron_in_point)
    {
        Vac_Bwron_in_Cnt++;
        if (Vac_Bwron_in_Cnt > Charging_Time)
        {
            // here need to save the bulk voltage for the saving data to do soft start ramp up
            PFC_Variables.VBulk_command = PFC_Variables.adc_raw[VBUS_CHANNEL];
            Vac_Bwron_in_Cnt = 0;
            Bwrom_IN_Flag = False;
            turn_on_pfc();
            PFC_Variables.supply_state = STATE_RAMP_UP;
        }
    }
    else
        Vac_Bwron_in_Cnt = 0;
}
// VBULK 爬升 送PGI致2次側
inline void ramp_up_state_handler(void)
{
    /*PFC soft start ramp up need to write here*/
    PFC_Variables.VBulk_command += PFC_Variables.ramp_up_step;
    /*Fix slew rate here*/
    if (PFC_Variables.VBulk_command > Boost_Slew_Rate)
        PFC_Variables.ramp_up_step = PFC_Variables.ramp_up_step >> 2;

    /*Vbulk OVP*/
    if (PFC_Variables.adc_raw[VBUS_CHANNEL] > OVer_Voltage_VBULK)
    {
        turn_off_pfc();
        PFC_Variables.supply_state = STATE_PFC_SHUT_DOWN;
    }

    /*等待PFC電壓升到對應Vac該值才開啟PFC*/
    if (PFC_Variables.adc_raw[VBUS_CHANNEL] > PFC_Variables.PFC_ON_Target)
    {
        PGI;                                       // SEND PGI
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
    case I_STATE_1: // 電壓環 TYPE3
       // proportional_integral(PFC_Variables.VBulk_command - PFC_Variables.adc_raw[VBUS_CHANNEL]);
        Volt_Loop_Controller();
        PFC_Variables.task_state = I_STATE_2;
        break;

    case I_STATE_2: // 

        PFC_Variables.task_state = I_STATE_3;
        break;

        case I_STATE_3: // AC 跌落
            AC_Drop_Method();
            PFC_Variables.task_state = I_STATE_5;
            break;

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

        //ADC Scan
        Multi_ADC_Sample();

        //負載變化條壓

        //Sweeping Ac 
        AC_Sweeping_Method();

        //PFC Task 
        PFC_TASK_STATE();
    }
}
