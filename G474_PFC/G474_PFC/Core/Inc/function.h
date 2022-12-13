#ifndef __FUNCTION_H_
#define __FUNCTION_H_
/************************include**************/
#include "systemsetting.h"

/*RTOS Sub Function */
void start_task(void *pvParameters);
void Task_1(void *pvParameters);
void Task_2(void *pvParameters);


/********************ADC_Function*******************/
// extern void ADC_Sample(void);
// extern void Multi_ADC_Sample(void); // multi sample
// /****************AC Cycle Calculate****************/
// extern inline void clear_positive_accumulators(void);
// extern inline void store_positive_cycle_values(void);
// extern inline void accumulate_positive_cycle_values(void);
// extern inline void clear_negative_accumulators(void);
// extern inline void store_negative_cycle_values(void);
// extern inline void accumulate_negative_cycle_values(void);

// extern inline void rectify_vac(void);
// extern inline void half_cycle_processing(void);
// /***********************PFC STATE******************/
// extern inline void supply_state_handler(void);
// /********************Task Supple State*************/
// extern void PFC_TASK_STATE(void);
// extern inline int32_t proportional_integral(int32_t error);
// /****************Sub Function Delcare**************/
// extern inline void idle_state_handler(void);
// extern inline void relay_bounce_state_handler(void);
// extern inline void ramp_up_state_handler(void);
// extern inline void pfc_on_state_handler(void);
// extern inline void pfc_shut_down_state_handler(void);
// extern inline void pfc_hiccup_state_handler(void);
// extern void turn_on_pfc(void);
// extern void turn_off_pfc(void);
// extern inline void Initail_Variable(void);




#endif
