/******************************************************************************
 *
 * [FILE NAME]:     app.h
 *
 * [AUTHOR]:        Omar Amr
 *
 * [DATE]:          02-11-2022
 *
 * [Description]:   Header file for Door lock Application (HMI ECU)
 *
 *******************************************************************************/

#ifndef APP_APP_H_
#define APP_APP_H_

#include "../HAL/LCD/lcd.h"
#include "../HAL/Keypad/keypad.h"
#include "../MCAL/USART/usart.h"
#include "../MCAL/Timer/timer.h"
#include <util/delay.h>
#include <avr/interrupt.h>

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define PASSWORD_LENGTH 			5		/*the exact number of digits that a user must enter*/
#define PASSWORD_ENTER_KEY			'='		/*the key used to enter the password*/
#define MATCHING_PASSWORD_BYTE		0xFF	/*byte received from CONTROL ECU when password is matching*/
#define UNMATCHING_PASSWORD_BYTE	0x00	/*byte received from CONTROL ECU when password not matching*/
#define ZERO_ASCII_CODE				48 		/*ascii-code of number 0*/
#define PRESS_TIME					150
#define MOTOR_ROTATION_DELAY		15000	/*time taken for the motor to open/close the door*/
#define DOOR_OPEN_TIME				3000	/*time for which the door is left open*/
#define MAX_WRONG_PASSWORDS			3		/*Allowed number of wrong passwords before alarm triggers*/
#define TIMER1_COMPARE_VALUE_7SEC	58594	/*compare value for timer1 to tick every 7.5 seconds*/
#define TIMER1_COMPARE_VALUE_3SEC	23438	/*compare value for timer1 to tick every 3 seconds*/
#define SCREEN_WRITE_DELAY			40
#define PASSWORD_CHARACHER			'*'

/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/

typedef enum
{
	CHANGE_PASS, DOOR_OPEN, ALARM
}APP_MainMenuData;

typedef enum{
	MATCHING_PASSWORDS, UNMATCHING_PASSWORDS
}APP_PasswordStatus;

typedef enum{
	NO_COMMAND,						/*No command was sent to CONTROL ECU*/
	OPEN_DOOR_COMMAND = 0x10,		/*Command sent to CONTROL ECU to open the door*/
	CHANGE_PASSWORD_COMMAND = 0x11,	/*Command sent to CONTROL ECU to change the password*/
	ALARM_COMMAND = 0x12			/*Command when alarm is to be triggered*/
}APP_Commands;


/*******************************************************************************
 *                           Program Scope Variables                           *
 *******************************************************************************/

extern uint8 g_wrong_passwords;

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

/*
 * Description:
 * Display welcome message at program start.
 * */
void APP_welcomeScreen(void);

/*
 * Description:
 * A function that sets a new password.
 * 1- It prompts the user to enter the new password and re-enter it to confirm.
 * 2- Sends the  two passwords to the CONTROL ECU
 * 3- It inquires the status of these password.
 * The function is executed in case of a New password or changing an existing one.
 * */
void APP_setNewPassword(void);

/*
 * Description:
 * Displays the main menu: prompts the user to make a choice.
 * Checks whether the password entered by user is correct or not.
 * returns the choice made by the user.
 * */
APP_MainMenuData APP_mainMenu(void);

/*
 * Description:
 * Sequence of steps that HMI_ECU does when opening the door:
 * 1- Send the door open command command to CONTROL ECU.
 * 2- Display the door status on LCD.
 * */
void APP_doorOpenSequence(TIMER_ConfigType * const a_timer1_config);

/*
 * Description:
 * It sends command to CONTROL ECU to change the password.
 * It then gets the password and its confirmation from the user.
 * */
void APP_changePasswordSequence();

/*
 * Description:
 * Sequence of steps that HMI_ECU does when opening the door:
 * 1- reset wrong passwords global counter .
 * 2- display error message on LCD screen.
 * 3- Do Not receive any input for 1 minute.
 * */
void APP_alarmSequence(TIMER_ConfigType * const a_timer1_config);

/*
 * Description :
 * Callback function  that increments a global variable g_timer1_tick
 */
void APP_timerTickIncrement(void);

#endif /* APP_APP_H_ */
