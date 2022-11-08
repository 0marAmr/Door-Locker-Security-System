/******************************************************************************
 *
 * [FILE NAME]:     app.h
 *
 * [AUTHOR]:        Omar Amr
 *
 * [DATE]:          02-11-2022
 *
 * [Description]:   Header file for Door lock Application (CONTROL ECU)
 *
 *******************************************************************************/

#ifndef APP_APP_H_
#define APP_APP_H_

#include "../MCAL/USART/usart.h"
#include "../MCAL/Timer/timer.h"
#include "../MCAL/I2C/twi.h"
#include "../HAL/Motors/DC_Motor/dc_motor.h"
#include "../HAL/Buzzer/buzzer.h"
#include "../HAL/EEPROM/eeprom_24c16.h"
#include <avr/interrupt.h>

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define PASSWORD_LENGTH 5
#define MATCHING_PASSWORD_BYTE		0xFF	/*byte sent to HMI ECU when password is matching*/
#define UNMATCHING_PASSWORD_BYTE	0x00	/*byte sent to HMI ECU when password not matching*/
#define MAX_WRONG_PASSWORDS			3		/*Allowed number of wrong passwords before alarm triggers*/
#define TIMER1_COMPARE_VALUE_7SEC	58594	/*compare value for timer1 to tick every 7.5 seconds*/
#define TIMER1_COMPARE_VALUE_3SEC	23438	/*compare value for timer1 to tick every 3 seconds*/
#define PASSWORD_BASE_ADDRESS		0x0200	/*The start address of the password location in EEPROM*/

/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/

typedef enum{
	MATCHING_PASSWORDS, UNMATCHING_PASSWORDS
}APP_PasswordStatus;

typedef enum{
	NO_COMMAND,						/*No command was received from HMI ECU*/
	OPEN_DOOR_COMMAND = 0x10,		/*Command received from HMI ECU to open the door*/
	CHANGE_PASSWORD_COMMAND = 0x11,	/*Command received from HMI ECU to change the password*/
	ALARM_COMMAND = 0x12			/*Command when alarm is to be triggered*/
}APP_Commands;


/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

/*
 * Description:
 * Function that receives a command from HMI ECU.
 * It returns the given command or NO_COMMAND if non is received (wrong pass).
 * */
APP_Commands APP_receiveCommand(void);

/*
 * Description:
 * Sequence of steps that CONTROL_ECU does when opening the door:
 * 1- rotates the motor CW for 15 seconds 	: Opens the Door
 * 2- turn off the motor for 3 seconds		: Hold the Door open
 * 3- rotates the motor ACW for 15 seconds 	: Closes the Door
 * */
void APP_doorOpenSequence(TIMER_ConfigType * const a_timer1_config);

/*
 * Description:
 * A function that changes the password.
 * The function is executed in case of a New password or changing an existing one.
 * */
void APP_changePassword(void);

/*
 * Description:
 * sets the buzzer for 1 minute.
 * */
void APP_alarmSequence(TIMER_ConfigType * const a_timer1_config);

/*
 * Description :
 * Callback function  that increments a global variable g_timer1_tick
 */
void APP_timerTickIncrement(void);

#endif /* APP_APP_H_ */
