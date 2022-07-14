#include"main.h"
#include"FeedBack_PI.h"


inline void proportional_integral(void)
{
     //Voltage control for P gain I gain,
     Voltage_state_error=Vref-Vout;
    
     Voltage_P_Gain+=Voltage_state_error*PERIOD_VALUE*System_Variable.pid.Voltage_Kp;

     Voltage_I_Gain=Voltage_P_Gain+Voltage_state_error*System_Variable.pid.Voltage_Ki;  //voltage P gain singal to current 

     Iref=Voltage_I_Gain*Voltage_FeedFoward_Gain;  //voltage singal after P gain I gain   Vce-*2Vo


     //Current control for P gain I gain 
     
     //Interleaved for P gain I gain test  

     Current_state_error=(Iref-IL1-IL2);  //Ierf-IL    avg ind current
  
     Current_P_Gain+=Current_state_error*PERIOD_VALUE*System_Variable.pid.Current_kp;  

     TIM1->CCR1=Current_P_Gain+Current_state_error*System_Variable.pid.Current_ki; //camp with carrier singal  contronl the pwm
	
	
}