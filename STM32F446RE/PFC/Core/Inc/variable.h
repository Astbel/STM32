#ifndef __VARIABLE_H_
#define __VARIABLE_H_
/************************include**************/
#include "systemsetting.h"

/*********************ADC*****************************/
extern uint16_t analog_result_volt;
extern float VBulk;
extern uint32_t ac_rect_sum;
extern float real_ac;
extern uint16_t Vac_temp;
extern uint16_t Vac_PP;
extern uint16_t Sample_cnt;
extern uint16_t Vac_plug_cnt;
/***********************Vac************************/
extern uint16_t Vac_peak;
extern uint16_t Vac_peak_temp;
extern uint32_t Bulk_Volt_Target;
extern uint32_t Power_good_PFC_Bulk_Target;
// extern uint16_t
/***********************STATE**********************/
typedef enum
{
    STATE_IDLE,
    STATE_RELAY_BOUNCE,
    STATE_RAMP_UP,
    STATE_PFC_ON,
    STATE_PFC_HICCUP,
    STATE_PFC_SHUT_DOWN,
    STATE_AC_DROP_RAMP_UP
} SUPPLY_STATE;

typedef enum
{
    I_STATE_1,
    I_STATE_2,
    I_STATE_3,
    I_STATE_4,
    I_STATE_5
} TASK_STATE;

//AC Channel
typedef enum 
{
   STATE_90, 
   STATE_115,
   STATE_230, 
   STATE_264

}AC_channel;

/********************STRUCT***************************/
struct PFC_VARIABLES
{
    uint32_t adc_raw[5];
    
    // Vac result
    uint32_t vin_raw;
    uint32_t vin_sum;
    uint32_t vin_filtered;
    uint32_t vin_squared;
    uint32_t vin_squared_slow_average;
    uint32_t vin_squared_average;
    /*5Vsb */
    uint32_t Standby_Charging_Target;
    /*Vac state */
    uint8_t AC_STATE;

    /*Vbulk*/
    uint32_t VBulk_command;
    /*Boost ramp up volt*/
    uint32_t ramp_up_step;

    /*Boost Voltage Target Command*/
    uint32_t PFC_ON_Target; 

    /*PFC STATE*/
    TASK_STATE task_state;
    SUPPLY_STATE supply_state;
};

/*******************Struct_Delcare*****************/
extern struct PFC_VARIABLES PFC_Variables;
extern struct PID PID;
/*********************Flag***************************/
extern uint8_t Bwrom_IN_Flag;

/*********************Counter**********************/
extern uint16_t Vac_Bwron_in_Cnt;

/*********************PID*************************/
typedef struct PID
{
    int32_t  Kp_limit;
    int32_t  Ki_limit;
    uint32_t kp;
    uint32_t ki;
    uint32_t kd;
};
/**********************DPWM***********************/
extern int32_t Voltage_Kp; 
extern int32_t Voltage_Ki;
extern int32_t steady_state_err;
extern int32_t obser_point;
extern int16_t I_temp;
extern int16_t D_temp;
extern uint16_t DPWM_TEMP;

/*****************Voltage Controller****************/
typedef struct Volt_Controller_3p3z
{
    //Array for input and output
    uint32_t Yn[4];
    uint32_t Xn[4];
    //Coeffiect for B is Input  
    uint32_t B0;
    uint32_t B1;
    uint32_t B2;
    uint32_t B3;
    //Coeffiect for A is output
    uint32_t A1;
    uint32_t A2;
    uint32_t A3;
    //Output to PWM register
    uint32_t Voltage_Loop_Out;
};

typedef struct AC_Drop
{
    //ac counter
    uint16_t ac_drop_cnt;

    //ac drop flag
    uint8_t ac_drop_happen_flag;

};






#endif
