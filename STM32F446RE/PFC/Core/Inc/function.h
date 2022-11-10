#ifndef __FUNCTION_H_
#define __FUNCTION_H_
/************************include**************/
#include "systemsetting.h"

/*RTOS Sub Function */
void start_task(void *pvParameters);
void Task_1(void *pvParameters);
void Task_2(void *pvParameters);

/*Task scheduler for mian loop*/
void start_task(void *pvParameters);

/********************ADC_Function*******************/
extern void ADC_Sample(void);

/***********************PFC STATE******************/
extern inline void supply_state_handler(void);
/********************Task Supple State*************/
extern void PFC_TASK_STATE(void);

/****************Sub Function Delcare**************/
extern inline void idle_state_handler(void);
extern inline void relay_bounce_state_handler(void);
extern inline void ramp_up_state_handler(void);
extern inline void pfc_on_state_handler(void);
extern inline void pfc_shut_down_state_handler(void);
extern inline void pfc_hiccup_state_handler(void);
extern void turn_on_pfc(void);
extern void turn_off_pfc(void);


#endif
