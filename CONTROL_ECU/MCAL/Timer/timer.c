/******************************************************************************
 *
 * [FILE NAME]:     timer.c
 *
 * [AUTHOR]:        Omar Amr
 *
 * [DATE]:          21-10-2022
 *
 * [Description]:   Source file for Timer driver
 *
 * [TARGET HW]:		ATmega32
 *
 *******************************************************************************/

#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

static volatile void (*g_timer0CallBackPtr) (void) = NULL_PTR;
static volatile void (*g_timer1CallBackPtr) (void) = NULL_PTR;
static volatile void (*g_timer2CallBackPtr) (void) = NULL_PTR;

/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/

ISR(TIMER0_OVF_vect){
	if(g_timer0CallBackPtr != NULL_PTR){
		(*g_timer0CallBackPtr)();
	}
}

ISR(TIMER0_COMP_vect){
	if(g_timer0CallBackPtr != NULL_PTR){
		(*g_timer0CallBackPtr)();
	}
}

ISR(TIMER1_OVF_vect){
	if(g_timer1CallBackPtr != NULL_PTR){
		(*g_timer1CallBackPtr)();
	}

}

ISR(TIMER1_COMPA_vect){
	if(g_timer1CallBackPtr != NULL_PTR){
		(*g_timer1CallBackPtr)();
	}
}


ISR(TIMER2_OVF_vect){
	if(g_timer2CallBackPtr != NULL_PTR){
		(*g_timer2CallBackPtr)();
	}
}

ISR(TIMER2_COMP_vect){
	if(g_timer2CallBackPtr != NULL_PTR){
		(*g_timer2CallBackPtr)();
	}
}

/*******************************************************************************
 *                      Functions Prototypes(Private)                          *
 *******************************************************************************/

static void TIMER0_init(TIMER_ConfigType * a_timerConfig);
static void TIMER1_init(TIMER_ConfigType * a_timerConfig);
static void TIMER2_init(TIMER_ConfigType * a_timerConfig);

static void TIMER0_deInit();
static void TIMER1_deInit();
static void TIMER2_deInit();

/*******************************************************************************
 *                    	  Functions Definitions                                *
 *******************************************************************************/


void TIMER_init(TIMER_ConfigType * a_timerConfig){
	switch(a_timerConfig->timer_id){
	case TIMER0_ID:
		TIMER0_init(a_timerConfig);
		break;
	case TIMER1_ID:
		TIMER1_init(a_timerConfig);
		break;
	case TIMER2_ID:
		TIMER2_init(a_timerConfig);
		break;
	}
}

void TIMER_deInit(TIMER_ID a_timerId){
	switch(a_timerId){
	case TIMER0_ID:
		TIMER0_deInit();
		break;
	case TIMER1_ID:
		TIMER1_deInit();
		break;
	case TIMER2_ID:
		TIMER2_deInit();
		break;
	}
}

void TIMER_setCallBackFunc(TIMER_ID a_timerId, void volatile (*a_functionAddressPtr) (void)){
	switch(a_timerId){
	case TIMER0_ID:
		g_timer0CallBackPtr = a_functionAddressPtr;
		break;
	case TIMER1_ID:
		g_timer1CallBackPtr = a_functionAddressPtr;
		break;
	case TIMER2_ID:
		g_timer2CallBackPtr = a_functionAddressPtr;
		break;
	}
}

void TIMER_changeCompareValue(TIMER_ID a_timerId, uint16 a_new_vlaue){
	switch(a_timerId){
	case TIMER0_ID:
		OCR0 = (uint8) a_new_vlaue;
		break;
	case TIMER1_ID:
		OCR1A =  a_new_vlaue;
		break;
	case TIMER2_ID:
		OCR2 = (uint8) a_new_vlaue;
		break;
	}
}

void TIMER_changeDutyCycle(TIMER_ConfigType * a_timerConfig){
	switch(a_timerConfig->timer_id){
	case TIMER0_ID:
		OCR0 = (((uint16)a_timerConfig->mode_data.pwm_duty_cycle * TIMER0_MAX_COUNT)/100);
		break;

	case TIMER1_ID:
		if(a_timerConfig->timer1_pwm_pin_select == PIN_OC1A){
			OCR1A = (((uint32)a_timerConfig->mode_data.pwm_duty_cycle * TOP_VALUE)/100);
		}
		else if(a_timerConfig->timer1_pwm_pin_select == PIN_OC1B){
			OCR1B = (((uint32)a_timerConfig->mode_data.pwm_duty_cycle * TOP_VALUE)/100);
		}
		break;

	case TIMER2_ID:
		OCR2 = (((uint16)a_timerConfig->mode_data.pwm_duty_cycle * TIMER2_MAX_COUNT)/100);
		break;
	}
}

static void TIMER0_init(TIMER_ConfigType * a_timerConfig){
	TCNT0 = 0;

	if(a_timerConfig->mode == PWM_MODE){
		OCR0 = (((uint16)a_timerConfig->mode_data.pwm_duty_cycle * TIMER0_MAX_COUNT)/100);
		DDRB  = DDRB | (1<<OC0); //set PB3/OC0 as output pin --> pin where the PWM signal is generated from MC.
	}
	else{
		SET_BIT(TCCR0,FOC0);
	}

	if(a_timerConfig->mode == OVERFLOW_MODE){
		SET_BIT(TIMSK,TOIE0);
		TCNT0 = a_timerConfig->mode_data.ovf_initial_value & 0x0F; //Set Timer Initial value
	}
	else if(a_timerConfig->mode == COMPARE_MODE){
		OCR0 = a_timerConfig->mode_data.ctc_compare_value;
		SET_BIT(TIMSK,OCIE0);
	}

	TCCR0 = ((a_timerConfig->mode & 0x01) << WGM00) |(a_timerConfig->ocx_pin_behavior << COM00)\
			| (GET_BIT(a_timerConfig->mode,1)<<WGM01) | ((a_timerConfig->prescaler.timer0 & 0x07) << CS00);

}

static void TIMER1_init(TIMER_ConfigType * a_timerConfig){
	TCNT1 = 0;
	ICR1 = TOP_VALUE;

	if(a_timerConfig->mode == PWM_MODE){
		if(a_timerConfig->timer1_pwm_pin_select == PIN_OC1A){
			OCR1A = (((uint32)a_timerConfig->mode_data.pwm_duty_cycle * TOP_VALUE)/100);
			TCCR1A |= (a_timerConfig->ocx_pin_behavior << COM1A0);
			DDRD  |= (1 << OC1A);
		}
		else if(a_timerConfig->timer1_pwm_pin_select == PIN_OC1B){
			OCR1B = (((uint32)a_timerConfig->mode_data.pwm_duty_cycle * TOP_VALUE)/100);
			TCCR1A |= (a_timerConfig->ocx_pin_behavior << COM1B0);
			DDRD  |= (1 << OC1B);
		}
	}
	else{
		SET_BIT(TCCR1A,FOC1A);
		SET_BIT(TCCR1A,FOC1B);
	}

	if(a_timerConfig->mode == OVERFLOW_MODE){
		SET_BIT(TIMSK,TOIE1);
		TCNT1 = a_timerConfig->mode_data.ovf_initial_value; //Set Timer Initial value
	}
	else if(a_timerConfig->mode == COMPARE_MODE){
		OCR1A = a_timerConfig->mode_data.ctc_compare_value;
		SET_BIT(TIMSK,OCIE1A);
	}

	TCCR1A |= (GET_BIT(a_timerConfig->mode,0) << WGM11);
	TCCR1B = (GET_BIT(a_timerConfig->mode,0) << WGM13) | (GET_BIT(a_timerConfig->mode,1) << WGM12)\
			| ( (a_timerConfig->prescaler.timer1 & 0x07) << CS10);

}

static void TIMER2_init(TIMER_ConfigType * a_timerConfig){
	TCNT2 = 0;

	if(a_timerConfig->mode == PWM_MODE){
		OCR2 = (((uint16)a_timerConfig->mode_data.pwm_duty_cycle * TIMER2_MAX_COUNT)/100);
		DDRD  |= (1<<OC2);
	}
	else{
		SET_BIT(TCCR2,FOC2);
	}

	if(a_timerConfig->mode == OVERFLOW_MODE){
		SET_BIT(TIMSK,TOIE2);
		TCNT2 = a_timerConfig->mode_data.ovf_initial_value & 0x0F; //Set Timer Initial value
	}
	else if(a_timerConfig->mode == COMPARE_MODE){
		OCR2 = a_timerConfig->mode_data.ctc_compare_value;
		SET_BIT(TIMSK,OCIE2);
	}

	TCCR2 = ((a_timerConfig->mode & 0x01) << WGM20) |(a_timerConfig->ocx_pin_behavior << COM20)\
			| (GET_BIT(a_timerConfig->mode,1)<<WGM21) | ((a_timerConfig->prescaler.timer2 & 0x07) << CS20);

}

static void TIMER0_deInit(){
	TCCR0 = 0;
	TCNT0 = 0;
	OCR0  = 0;
	CLEAR_BIT(TIMSK,OCIE0);
	CLEAR_BIT(TIMSK,TOIE0);
}

static void TIMER1_deInit(){
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1 = 0;
	OCR1A = 0;
	OCR1B = 0;
	ICR1 = 0;
	CLEAR_BIT(TIMSK,OCIE1A);
	CLEAR_BIT(TIMSK,OCIE1B);
	CLEAR_BIT(TIMSK,TOIE0);
}

static void TIMER2_deInit(){
	TCCR2 = 0;
	TCNT2 = 0;
	OCR2  = 0;
	CLEAR_BIT(TIMSK,OCIE2);
	CLEAR_BIT(TIMSK,TOIE2);
}

