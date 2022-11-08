/******************************************************************************
 *
 * [FILE NAME]:     keypad.h
 *
 * [AUTHOR]:        Omar Amr
 *
 * [DATE]:          11-10-2022
 *
 * [DESCRIPTION]:   Header file for keypad Driver
 *
 *******************************************************************************/

#ifndef HAL_KEYPAD_KEYPAD_H_
#define HAL_KEYPAD_KEYPAD_H_

#include "../../Utils/std_types.h"
#include "../../Utils/common_macros.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

/* Keypad static configurations*/
#define CUSTOM_KEYPAD			/*Special keypad with buttons configured below (configured)*/
/*#define STANDARD_KEYPAD 		*//*Keypad with no labels (outputs button number)*/
/*#define COMMERTIAL_KEYPAD 		*//*the typical keypad found in market */

/* Keypad configurations for number of rows and columns */
#define KEYPAD_NUM_COLS                  4
#define KEYPAD_NUM_ROWS                  4

/* Keypad Port Configurations */
#define KEYPAD_PORT_ID                   PORTB_ID

#define KEYPAD_FIRST_ROW_PIN_ID           PIN0_ID
#define KEYPAD_FIRST_COLUMN_PIN_ID        PIN4_ID

/* Keypad button logic configurations */
#define KEYPAD_BUTTON_PRESSED            LOGIC_LOW
#define KEYPAD_BUTTON_RELEASED           LOGIC_HIGH


/*******************************************************************************
 *                           Keypad Configurations                             *
 *******************************************************************************/

/*Special keypad with buttons configured below (configured)*/
#ifdef CUSTOM_KEYPAD
	enum{
		KEYPAD_BUTTON_1= 	7,
		KEYPAD_BUTTON_2 = 	8,
		KEYPAD_BUTTON_3 = 	9,
		KEYPAD_BUTTON_4 = 	'/', 	/* ASCII Code of / */
		KEYPAD_BUTTON_5 = 	4,
		KEYPAD_BUTTON_6 = 	5,
		KEYPAD_BUTTON_7 = 	6,
		KEYPAD_BUTTON_8 = 	'*', 	/* ASCII Code of '*' */
		KEYPAD_BUTTON_9 = 	1,
		KEYPAD_BUTTON_10 = 	2,
		KEYPAD_BUTTON_11 = 	3,
		KEYPAD_BUTTON_12 = 	'-',	/* ASCII Code of '-' */
		KEYPAD_BUTTON_13 = 	13,
		KEYPAD_BUTTON_14 = 	0,
		KEYPAD_BUTTON_15 = 	'=', 	/* ASCII Code of '=' */
		KEYPAD_BUTTON_16 = 	'+'		/* ASCII Code of '+' */
	};

#else

/*Keypad with no labels (outputs button number)*/
#ifdef STANDARD_KEYPAD
	enum{
		KEYPAD_BUTTON_1= 1,KEYPAD_BUTTON_2,KEYPAD_BUTTON_3,KEYPAD_BUTTON_4,KEYPAD_BUTTON_5,
		KEYPAD_BUTTON_6,KEYPAD_BUTTON_7,KEYPAD_BUTTON_8,KEYPAD_BUTTON_9, KEYPAD_BUTTON_10,
		KEYPAD_BUTTON_11,KEYPAD_BUTTON_12,KEYPAD_BUTTON_13,KEYPAD_BUTTON_14,KEYPAD_BUTTON_15,
		KEYPAD_BUTTON_16
	};

#else

/*the typical keypad found in market */
#ifdef COMMERTIAL_KEYPAD

	#if (KEYPAD_NUM_COLS == 3)
		enum{
			KEYPAD_BUTTON_1 = 1,KEYPAD_BUTTON_2,KEYPAD_BUTTON_3,KEYPAD_BUTTON_4,KEYPAD_BUTTON_5,
			KEYPAD_BUTTON_6,KEYPAD_BUTTON_7,KEYPAD_BUTTON_8,KEYPAD_BUTTON_9,
			KEYPAD_BUTTON_10 = '*',
			KEYPAD_BUTTON_11 = 0,
			KEYPAD_BUTTON_12 = '#'
		};

	#elif (KEYPAD_NUM_COLS == 4)
		enum{
			KEYPAD_BUTTON_1 = 1,KEYPAD_BUTTON_2,KEYPAD_BUTTON_3,
			KEYPAD_BUTTON_4 = 'A',
			KEYPAD_BUTTON_5 = 4,KEYPAD_BUTTON_6,KEYPAD_BUTTON_7,
			KEYPAD_BUTTON_8 = 'B',
			KEYPAD_BUTTON_9 = 7,KEYPAD_BUTTON_10,KEYPAD_BUTTON_11,
			KEYPAD_BUTTON_12 = 'C',
			KEYPAD_BUTTON_13 = '*',
			KEYPAD_BUTTON_14 = 0 ,
			KEYPAD_BUTTON_15 = '#',
			KEYPAD_BUTTON_16 = 'D'
		};

	#endif /* KEYPAD_NUM_COLS */

#else
#error "Wrong Keypad configuration"

#endif /* COMMERTIAL_KEYPAD */
#endif /* STANDARD_KEYPAD*/
#endif /* CUSTOM_KEYPAD */

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Get the Keypad pressed button
 */
uint8 KEYPAD_getPressedKey(void);

#endif /* HAL_KEYPAD_KEYPAD_H_ */
