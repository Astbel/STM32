#ifndef __VARIABLE_H_
#define __VARIABLE_H_
/************************include**************/

/******************結構體************************/
extern struct Volt_Controller_3p3z Volt_comtroller_3p3z;
extern struct PFC_VARIABLES PFC_Variables;
extern struct AC_Drop AC_Drop_class;
extern struct Allert_Portect Dyanmic_Portect;

/*********************ADC*****************************/

/***********************Vac************************/

/***********************Flash**********************/
extern uint32_t FirstSector, NbOfSectors, Address, SECTORError;
extern __IO uint32_t data32, MemoryProgramStatus;
/********************STRUCT***************************/
struct PFC_VARIABLES
{
    uint32_t adc_raw[5];
};
typedef struct Allert_Portect
{
    uint16_t Protect_12V;
    uint16_t Portect_5V;
};

/*********************Flag***************************/

/*********************Counter**********************/

/*********************PID*************************/

/**********************DPWM***********************/

/*****************Voltage Controller****************/
typedef struct Volt_Controller_3p3z
{
    // Array for input and output
    uint32_t Yn[4];
    uint32_t Xn[4];
    // Coeffiect for B is Input
    uint32_t B0;
    uint32_t B1;
    uint32_t B2;
    uint32_t B3;
    // Coeffiect for A is output
    uint32_t A1;
    uint32_t A2;
    uint32_t A3;
    // Output to PWM register
    uint32_t Voltage_Loop_Out;
};

typedef struct AC_Drop
{
    // ac counter
    uint16_t ac_drop_cnt;

    // ac drop flag
    uint8_t ac_drop_happen_flag;
};

#endif
