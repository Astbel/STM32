#ifndef __VARIABLE_H_
#define __VARIABLE_H_
/************************include**************/
#include "systemsetting.h"

/*********************ADC*****************************/
extern uint16_t analog_result_volt;
extern float VBulk;
extern uint16_t ADC_Result[3];
extern float adc_sample1;
extern float adc_sample2;

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

/*********************Flag***************************/
extern uint8_t Bwrom_IN_Flag;

/*********************Counter**********************/
extern uint16_t Vac_Bwron_in_Cnt;

/*********************PID*************************/

#endif
