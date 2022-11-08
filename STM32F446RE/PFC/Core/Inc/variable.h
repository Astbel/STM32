#ifndef __VARIABLE_H_
#define __VARIABLE_H_
/************************include**************/
#include "systemsetting.h"


/*********************ADC*****************************/
extern uint16_t analog_result_volt;
extern float VBulk;

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
    //Vac result
    uint32_t  vin_raw;
    uint32_t  vin_sum;
    uint32_t  vin_filtered;
    uint32_t  vin_squared;
    uint32_t  vin_squared_slow_average;
    uint32_t  vin_squared_average;

    /*PFC STATE*/
    TASK_STATE   task_state;
    SUPPLY_STATE supply_state;
};

/*******************Struct_Delcare*****************/
extern struct PFC_VARIABLES PFC_Variables;

/*********************Flag***************************/
extern uint8_t  Bwrom_IN_Flag;

/*********************Counter**********************/
extern uint16_t Vac_Bwron_in_Cnt;






































#endif
