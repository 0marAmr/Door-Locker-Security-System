/******************************************************************************
 *
 * [FILE NAME]:     keypad.h
 *
 * [AUTHOR]:        Omar Amr
 *
 * [DATE]:          11-10-2022
 *
 * [DESCRIPTION]:   Source file for keypad Driver
 *
 *******************************************************************************/

#include "../../MCAL/GPIO/gpio.h"
#include "keypad.h"
#include <util/delay.h>

/*******************************************************************************
 *                      Functions Prototypes(Private)                          *
 *******************************************************************************/

/*
 * Description :
 * Get the keypad pressed button value according to the selected configuration.
 */
static uint8 getButtonChar(uint8 button_number);

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
uint8 KEYPAD_getPressedKey(void)
{
	uint8 col,row;
	uint8 keypad_port_value = 0;
	while(1)
	{
		for(col=0;col<KEYPAD_NUM_COLS;col++) /* loop for columns */
		{
			/*
			 * Each time setup the direction for all keypad port as input pins,
			 * except this column will be output pin
			 */
			GPIO_setupPortDirection(KEYPAD_PORT_ID,PORT_INPUT);
			GPIO_setupPinDirection(KEYPAD_PORT_ID,KEYPAD_FIRST_COLUMN_PIN_ID+col,PIN_OUTPUT);

#if(KEYPAD_BUTTON_PRESSED == LOGIC_LOW)
			/* Clear the column output pin and set the rest pins value */
			keypad_port_value = ~(1<<(KEYPAD_FIRST_COLUMN_PIN_ID+col));
#else
			/* Set the column output pin and clear the rest pins value */
			keypad_port_value = (1<<(KEYPAD_FIRST_COLUMN_PIN_ID+col));
#endif
			GPIO_writePort(KEYPAD_PORT_ID,keypad_port_value);

			for(row=0;row<KEYPAD_NUM_ROWS;row++) /* loop for rows */
			{
				/* Check if the switch is pressed in this row */
				if(GPIO_readPin(KEYPAD_PORT_ID,row+KEYPAD_FIRST_ROW_PIN_ID) == KEYPAD_BUTTON_PRESSED)
				{

					return getButtonChar((row*KEYPAD_NUM_COLS)+col+1);
				}
			}
		}
	}
}


/*
 * Description :
 * Get the keypad pressed button value according to the selected configuration.
 */
static uint8 getButtonChar(uint8 button_number){
	uint8 button_character;
	switch(button_number){
	case 1:
		button_character = KEYPAD_BUTTON_1;
		break;
	case 2:
		button_character = KEYPAD_BUTTON_2;
		break;
	case 3:
		button_character = KEYPAD_BUTTON_3;
		break;
	case 4:
		button_character = KEYPAD_BUTTON_4;
		break;
	case 5:
		button_character = KEYPAD_BUTTON_5;
		break;
	case 6:
		button_character = KEYPAD_BUTTON_6;
		break;
	case 7:
		button_character = KEYPAD_BUTTON_7;
		break;
	case 8:
		button_character = KEYPAD_BUTTON_8;
		break;
	case 9:
		button_character = KEYPAD_BUTTON_9;
		break;
	case 10:
		button_character = KEYPAD_BUTTON_10;
		break;
	case 11:
		button_character = KEYPAD_BUTTON_11;
		break;
	case 12:
		button_character = KEYPAD_BUTTON_12;
		break;
#if (KEYPAD_NUM_COLS == 4)
	case 13:
		button_character = KEYPAD_BUTTON_13;
		break;
	case 14:
		button_character = KEYPAD_BUTTON_14;
		break;
	case 15:
		button_character = KEYPAD_BUTTON_15;
		break;
	case 16:
		button_character = KEYPAD_BUTTON_16;
		break;
#endif
	default:
		button_character = button_number;
		break;
	}
	return button_character;
}
