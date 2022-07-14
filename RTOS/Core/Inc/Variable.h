#ifndef VARIABLE_H_
#define VARIABLE_H_


#include "main.h"
//Vref Iref  sumaltion AVG control variable
#define Vref  (12)
#define IL1   (3)
#define IL2   (3)

//Delcare PiD 
extern uint16_t Voltage_FeedFoward_Gain;
extern uint32_t output;
extern uint32_t steady_state_error;
extern uint32_t Voltage_state_error;
extern uint32_t Current_state_error;
extern uint32_t Voltage_P_Gain;
extern uint32_t Voltage_I_Gain;
extern uint32_t Current_P_Gain;
extern uint32_t Curren_I_Gain;
extern uint16_t Iref;

//PID variable
struct PID_Variable
{
    uint32_t Voltage_Kp;
    uint32_t Voltage_Ki;
    uint32_t Current_kp;
    uint32_t Current_ki;
};

//Variable in system
struct System_Variable
{
   struct PID_Variable pid;
};

extern struct System_Variable  System_Variable;

//UART receive value 
extern uint16_t Vout;










































#endif
