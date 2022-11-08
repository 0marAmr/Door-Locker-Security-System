 /******************************************************************************
 *
 * [FILE NAME]:     buzzer.c
 *
 * [AUTHOR]:        Omar Amr
 *
 * [DATE]:          02-11-2022
 *
 * [Description]:   Source file for the buzzer driver
 *
 *******************************************************************************/

#include "../../MCAL/GPIO/gpio.h"
#include "buzzer.h"

void BUZZER_init(void)
{
	GPIO_setupPinDirection(BUZZER_PORT_ID, BUZZER_PIN_ID, PIN_OUTPUT);
	GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID, LOGIC_LOW);
}

void BUZZER_start(void)
{
	GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID, LOGIC_HIGH);
}

void BUZZER_stop(void)
{
	GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID, LOGIC_LOW);
}
