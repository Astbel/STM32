#include"main.h"
#include"FeedBack_PI.h"
#include "Variable.h"

//Delcare variable
struct System_Variable  System_Variable;
uint16_t Voltage_FeedFoward_Gain;
uint32_t output;
uint32_t steady_state_error;
uint32_t Voltage_state_error;
uint32_t Current_state_error;
uint32_t Voltage_P_Gain;
uint32_t Voltage_I_Gain;
uint32_t Current_P_Gain;
uint32_t Curren_I_Gain;
uint16_t Iref;



void proportional_integral(uint16_t Vo)
{
     System_Variable.pid.Voltage_Kp=11000;
     System_Variable.pid.Voltage_Ki=100;

     //Voltage control for P gain I gain,
     Voltage_state_error=Vref-Vo;
    
     Voltage_P_Gain+=Voltage_state_error*1/PERIOD_VALUE*System_Variable.pid.Voltage_Kp;

     Voltage_I_Gain=Voltage_P_Gain+Voltage_state_error*System_Variable.pid.Voltage_Ki;  //voltage P gain singal to current 

     //Feed back to pwm signal
	TIM1->CCR1 =Voltage_I_Gain;
	TIM1->CCR2 =Voltage_I_Gain;

     //Limit PWM output
     if ((TIM1->CCR1>PWM_OUTPUT_Upper_Limit)&&(TIM1->CCR2>PWM_OUTPUT_Upper_Limit))
     {
          TIM1->CCR1 =PWM_OUTPUT_Upper_Limit;
	     TIM1->CCR2 =PWM_OUTPUT_Upper_Limit;
     }
     else if ((TIM1->CCR1<PWM_OUTPUT_Lowwer_Limit)&&(TIM1->CCR2<PWM_OUTPUT_Lowwer_Limit))
     {
          TIM1->CCR1 =PWM_OUTPUT_Lowwer_Limit;
	     TIM1->CCR2 =PWM_OUTPUT_Lowwer_Limit;
     }
     
     
}