/******************************************************************************
 *
 * [FILE NAME]:     timer.h
 *
 * [AUTHOR]:        Omar Amr
 *
 * [DATE]:          21-10-2022
 *
 * [Description]:   Header file for Timer driver
 *
 * [TARGET HW]:		ATmega32
 *
 *******************************************************************************/

#ifndef _TIMER_H_
#define _TIMER_H_

#include "../../Utils/std_types.h"
#include "../../Utils/common_macros.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define TIMER0_MAX_COUNT 255
#define TIMER1_MAX_COUNT 65536
#define TIMER2_MAX_COUNT 255
#define TOP_VALUE 4783

#define OC0 	PB3
#define OC1B 	PD4
#define OC1A 	PD5
#define OC2 	PD7

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/

typedef enum{
	TIMER0_ID, TIMER1_ID, TIMER2_ID,
}TIMER_ID;

/*Pre-scaler definitions for timer 0*/
typedef enum{
	TIMER0_NO_CLK, TIMER0_F_CPU_1, TIMER0_F_CPU_8, TIMER0_F_CPU_64, TIMER0_F_CPU_256, TIMER0_F_CPU_1024,
	TIMER0_EXTERNAL_CLK_FALLING, TIMER0_EXTERNAL_CLK_RISING
}TIMER0_Prescaler;

/*Pre-scaler definitions for timer 1*/
typedef enum{
	TIMER1_NO_CLK, TIMER1_F_CPU_1, TIMER1_F_CPU_8, TIMER1_F_CPU_64, TIMER1_F_CPU_256, TIMER1_F_CPU_1024,
	TIMER1_EXTERNAL_CLK_FALLING, TIMER1_EXTERNAL_CLK_RISING
}TIMER1_Prescaler;

/*pre-sclaer definitions for timer 2*/
typedef enum{
	TIMER2_NO_CLK, TIMER2_F_CPU_1, TIMER2_F_CPU_8, TIMER2_F_CPU_32, TIMER2_F_CPU_64,TIMER2_F_CPU_128,
	TIMER2_F_CPU_256, TIMER2_F_CPU_1024
}TIMER2_Prescaler;

typedef enum{
	OVERFLOW_MODE, COMPARE_MODE = 2, PWM_MODE
}TIMER_Mode;

typedef enum{
	DISCONNECT_OCX, TOGGLE_OCX, CLEAR_OCX, SET_OCX
}TIMER_CompareOutput;

/*timer1 output compare pins definition*/
typedef enum{
	PIN_OC1A , PIN_OC1B
}TIMER1_OutputPinSelect;

/*A union containing the data specific for each mode of operation of timers*/
typedef union{
	uint16 ovf_initial_value;		/*Overflow Mode*/
	uint16 ctc_compare_value;		/*CTC Mode*/
	uint8  pwm_duty_cycle;			/*FAST PWM mode*/
}TIMER_ModeData;

/*A union containing the pre-scalers for timers (timer2 has a wider pre-scaling range)*/
typedef union{
	TIMER0_Prescaler timer0;
	TIMER1_Prescaler timer1;
	TIMER2_Prescaler timer2;
}TIMER_GeneralPrescaler;

typedef struct{
	TIMER_ModeData timer_mode_data;
	TIMER_ID timer_id;
	TIMER_Mode timer_mode;
	TIMER_GeneralPrescaler timer_prescaler;
	TIMER_CompareOutput timer_ocx_pin_behavior;
	TIMER1_OutputPinSelect timer1_pwm_pin_select; /*Special config. for timer1 to select compare output pins*/
}TIMER_ConfigType;

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

void TIMER_init(TIMER_ConfigType * a_timerConfig);
void TIMER_deInit(TIMER_ID a_timerId);
void TIMER_setCallBackFunc(TIMER_ID a_timerId, void volatile (*a_functionAddressPtr) (void));
void TIMER_changeCompareValue(TIMER_ID a_timerId, uint16 a_new_vlaue);

#endif /* TIMER_H_ */
