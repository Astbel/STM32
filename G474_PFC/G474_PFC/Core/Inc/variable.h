#ifndef __VARIABLE_H_
#define __VARIABLE_H_
/************************include**************/
#include "systemsetting.h"

/*********************ADC*****************************/
extern uint16_t analog_result_volt;
extern float VBulk;
// extern uint16_t ADC_Result[ADC_Sample_Rate];
// extern float adc_sample1;
// extern float adc_sample2;
extern uint32_t test_ac_sample;
extern uint32_t ac_sum;
extern float real_ac;
extern uint16_t Vac_temp;
extern uint16_t Vac_PP;
extern uint16_t Sample_cnt;
extern uint16_t adc_sample1;
extern uint16_t adc_sample2;
// multi channel
extern uint16_t ADC_SAMPLE_ARR[2];

/***********************Vac************************/
extern uint16_t Vac_peak;
extern uint16_t Vac_peak_temp;
// extern uint16_t
/***********************STATE**********************/
typedef enum
{
    STATE_IDLE,
    STATE_RELAY_BOUNCE,
    STATE_RAMP_UP,
    STATE_PFC_ON,
    STATE_PFC_HICCUP,
    STATE_PFC_HICCUP1,
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

/********************STRUCT***************************/
struct PFC_VARIABLES
{
    uint32_t adc_raw[5];
    uint32_t adc_avg[5];

    // Vac result
    uint32_t vin_raw;
    uint32_t vin_sum;
    uint32_t vin_filtered;
    uint32_t vin_squared;
    uint32_t vin_squared_slow_average;
    uint32_t vin_squared_average;

    /*半周計算*/
    /*負半周*/
    uint32_t negative_cycle_count;
    uint32_t negative_cycle_counter;
    uint32_t negative_vin_squared_accumulate;

    /*正半周*/
    uint32_t positive_cycle_count;
    uint32_t positive_cycle_counter;
    uint32_t positive_vin_squared_accumulate;

    /*Vac 跌落*/
    uint32_t ac_drop_count;
    uint32_t ac_drop;
    uint32_t ac_drop_recovery_not_complete;
    uint32_t vin_squared_for_ac_drop;

    /*Vac state */
    uint8_t AC_STATE;

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

#endif
