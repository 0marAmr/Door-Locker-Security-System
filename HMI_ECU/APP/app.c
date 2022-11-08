/******************************************************************************
 *
 * [FILE NAME]:     app.c
 *
 * [AUTHOR]:        Omar Amr
 *
 * [DATE]:          02-11-2022
 *
 * [Description]:   Source file for Door lock Application (HMI ECU)
 *
 *******************************************************************************/

#include "app.h"

/*******************************************************************************
 *                     	   	  Global Variables                                 *
 *******************************************************************************/

/*the  entered password (extra character for the terminator special char)*/
uint8 g_passwordInput[PASSWORD_LENGTH + 1] = {0};
uint8 g_wrong_passwords = 0;	/*wrong passwords counter*/
uint8 g_timer1_tick = 0;	/*timer 1 compare match counter*/


/*******************************************************************************
 *                      Functions Prototypes(Private)                          *
 *******************************************************************************/

/*
 * Description:
 * Function that sends a given command to CONTROL ECU
*/
static void APP_sendCommand(uint8 a_command);

/*
 * Description:
 * prompts the user a given instruction on the LCD.
 * get the input from the keypad and store it.
*/
static void APP_getPassword(const uint8 const * a_user_prompt);

/*
 * Description:
 * prompts the user to enter the new password and re-enter it to confirm.
 * send both passwords to the CONTROL ECU to be compared and saved.
*/
static void APP_sendPassword(void);

/*
 * Description:
 * A generic function that confirms whether the entered password matches:
 * 1- the re-entered password in case of password change/initialization
 * 2- the actual password stored in the EEPROM in case of a door access or password change.
*/
static APP_PasswordStatus APP_passwordEnquire(void);


/*******************************************************************************
 *                     		 Functions Definitions                             *
 *******************************************************************************/

/*
 * Description:
 * prompts the user a given instruction on the LCD.
 * get the input from the keypad and store it.
*/
static void APP_getPassword(const uint8 const * a_user_prompt)
{
	uint8 key , i = 0;
	LCD_displayStringRowColumn(0,0,a_user_prompt);

	LCD_moveCursor(1,13); /* Move the cursor to the second row */
	LCD_sendCommand(LCD_CURSOR_BLINK);

	/*keep getting input until 5 digits are entered.
	 * eventually, only exit when equal (=) key is pressed on keypad)*/
	while(i<PASSWORD_LENGTH || key != PASSWORD_ENTER_KEY){
		key = KEYPAD_getPressedKey();

		/*accept numeric inputs only*/
		if((key <= 9) && (key >= 0) && i<PASSWORD_LENGTH)
		{
			/*store the ascii-code of each number in a global variable*/
			g_passwordInput[i] = ZERO_ASCII_CODE + key;
			LCD_displayCharacter('*');   /* display an asterisk (*) for each digit entered */
			i++;
		}
		_delay_ms(PRESS_TIME); /*delay for button press*/

		/*Turn off the cursor when 5 characters are entered*/
		if(i==PASSWORD_LENGTH)
		{
			LCD_sendCommand(LCD_CURSOR_OFF);
		}
	}

	g_passwordInput[5] = USART_TERMINATOR_CHARACTER;  /*A special character denoting the end of a string for UART_send*/

	LCD_clearScreen(); /*clear the LCD*/
}

/*
 * Description:
 * prompts the user to enter the new password and re-enter it to confirm.
 * send both passwords to the CONTROL ECU to be compared and saved.
*/
static void APP_sendPassword(void)
{
	/*Acknowledge that sender (HMI ECU) is ready to send password value */
	USART_sendByte(USART_SENDER_READY_BYTE);

	while( USART_receiveByte() != USART_RECEIVER_READY_BYTE)
	{
		; /*wait until the receiver (CONTROL ECU) is ready to receive*/
	}

	USART_sendString(g_passwordInput); /*send the password to receiver (CONTROL ECU)*/
}

/*
 * Description:
 * A generic function that confirms whether the entered password matches:
 * 1- the re-entered password in case of password change/initialization
 * 2- the actual password stored in the EEPROM in case of a door access or password change.
*/
static APP_PasswordStatus APP_passwordEnquire(void)
{
	/*variable to store the received password_status from uart*/
	uint8 received_compare_result;
	/*Acknowledge that receiver (HMI ECU) is ready to receive password compare_result */
	USART_sendByte(USART_RECEIVER_READY_BYTE);

	while( USART_receiveByte() != USART_SENDER_READY_BYTE)
	{
		; /*wait until the sender (CONTROL) is ready to send password compare_result  */
	}

	/*get the received status from UART*/
	received_compare_result = USART_receiveByte();

	//	LCD_intgerToString(received);

	/*if the two entered passwords are matching*/
	if(received_compare_result == MATCHING_PASSWORD_BYTE)
	{
		return MATCHING_PASSWORDS;
	}
	/*if the two entered passwords are not matching, prompt to the user*/
	else if (received_compare_result == UNMATCHING_PASSWORD_BYTE && g_wrong_passwords !=3)
	{
		LCD_displayStringRowColumn(0,0,"ERROR: Password Does Not Match.");
		LCD_displayStringRowColumn(1,0,"Please Try Again !");
		_delay_ms(1000);
		LCD_clearScreen();
		return UNMATCHING_PASSWORDS;
	}
	else
		return UNMATCHING_PASSWORDS;
}

/*
 * Description:
 * Function that sends a given command to CONTROL ECU
*/
static void APP_sendCommand(uint8 a_command){
	/*Acknowledge that sender (HMI ECU) is ready to send COMMAND*/
	USART_sendByte(USART_SENDER_READY_BYTE);

	while( USART_receiveByte() != USART_RECEIVER_READY_BYTE){
		; /*wait until the receiver (CONTROL ECU) is ready to receive the COMMAND*/
	}

	USART_sendByte(a_command); /*send a command to signal the door unlocking sequence*/
}

/*
 * Description:
 * Display welcome message at program start.
 * */
void APP_welcomeScreen(void)
{
	LCD_sendCommand(LCD_CURSOR_BLINK);
	LCD_moveCursor(0, 6);
	LCD_writeString("DOOR SECURITY SYSTEM", SCREEN_WRITE_DELAY);
	LCD_moveCursor(1, 10);
	LCD_writeString("By: Omar Hafz", SCREEN_WRITE_DELAY);
	LCD_sendCommand(LCD_CURSOR_OFF);
	_delay_ms(1000);
	LCD_clearScreen();
}

/*
 * Description:
 * A function that sets a new password.
 * 1- It prompts the user to enter the new password and re-enter it to confirm.
 * 2- Sends the  two passwords to the CONTROL ECU
 * 3- It inquires the status of these password.
 * The function is executed in case of a New password or changing an existing one.
*/
void APP_setNewPassword(void)
{
	/*ask the user to initialize a password until the two entered passwords matches*/
	do
	{
		/*the new password*/
		APP_getPassword("Please Enter A New Password:"); 	/*get the password input from user*/
		APP_sendPassword();

		/*confirm the new password*/
		APP_getPassword("Please Re-enter The Password:"); 	/*get the password input from user*/
		APP_sendPassword();
	}
	while(APP_passwordEnquire() == UNMATCHING_PASSWORDS);
}

/*
 * Description:
 * Displays the main menu: prompts the user to make a choice.
 * Checks whether the password entered by user is correct or not.
 * returns the choice made by the user.
*/
APP_MainMenuData APP_mainMenu(void)
{
	uint8 key;
	LCD_displayStringRowColumn(0, 0, "(+): Open The Door.");
	LCD_displayStringRowColumn(1, 0, "(-): Change The Password.");

	/*wait for user to choose whether to open the door or change the password*/
	do
	{
		key = KEYPAD_getPressedKey();
	}
	while(key != '+' && key != '-');

	LCD_clearScreen();

	do
	{
		g_wrong_passwords++;
		if(g_wrong_passwords > MAX_WRONG_PASSWORDS) /*the user has up to 3 trails*/
		{
			return ALARM;
		}
		APP_getPassword("Please Enter The Password:");		/*ask the user to enter a password*/
		APP_sendPassword();								/*send it to CONTROL ECU to be checked*/
		}
	while(APP_passwordEnquire() == UNMATCHING_PASSWORDS);

	/*reset the counter if a correct password is entered*/
	g_wrong_passwords = 0;

	if (key == '-')
	{
		return CHANGE_PASS;

	}
	else
	{
		return DOOR_OPEN;
	}
}

/*
 * Description :
 * Callback function  that increments a global variable g_timer1_tick
 */
void APP_timerTickIncrement(void)
{
	g_timer1_tick++;
}

/*
 * Description:
 * Sequence of steps that HMI_ECU does when opening the door:
 * 1- Send the door open command command to CONTROL ECU.
 * 2- Display the door status on LCD.
*/
void APP_doorOpenSequence(TIMER_ConfigType * const a_timer1_config)
{
	APP_sendCommand(OPEN_DOOR_COMMAND);

	/*Display the door status on LCD*/
	/*Display the door opening string for 15 seconds*/
	LCD_displayStringRowColumn(0, 0, "The Door is Opening...");
	a_timer1_config->timer_mode_data.ctc_compare_value = TIMER1_COMPARE_VALUE_7SEC;
	TIMER_init(a_timer1_config);
	while(g_timer1_tick < 2);

	/*Display the door is opened for 3 seconds*/
	LCD_displayStringRowColumn(0, 0, "The Door is Opened !  ");
	TIMER_changeCompareValue(TIMER1_ID, TIMER1_COMPARE_VALUE_3SEC);
	while(g_timer1_tick < 3);

	/*Display the door closing string for 15 seconds*/
	LCD_displayStringRowColumn(0, 0, "The Door is Closing...");
	TIMER_changeCompareValue(TIMER1_ID, TIMER1_COMPARE_VALUE_7SEC);
	while(g_timer1_tick < 5);
	TIMER_deInit(TIMER1_ID);
	g_timer1_tick = 0;
}

/*
 * Description:
 * It sends command to CONTROL ECU to change the password.
 * It then gets the password and its confirmation from the user.
*/
void APP_changePasswordSequence()
{
	APP_sendCommand(CHANGE_PASSWORD_COMMAND); /*send command to CONTROL ECU*/
	APP_setNewPassword(); /*get password and confirmation*/
	LCD_displayStringRowColumn(0, 0, "The New Password Is Now Active:)");
	_delay_ms(1500);
	LCD_clearScreen();
}

/*
 * Description:
 * Sequence of steps that HMI_ECU does when an alarm is triggered.
 * It's executed when the global wrong password counter is 3.
*/
void APP_alarmSequence(TIMER_ConfigType * const a_timer1_config)
{
	g_wrong_passwords = 0;/*reset wrong passwords global counter*/

	/*display error message on LCD screen*/
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "ERROR: TOO MANY ATTEMPTS !");
	LCD_displayStringRowColumn(1, 0, "DOOR IS LOCKED FOR 1 MIN..");

	/*display the message for 1 minute via timer 1*/
	a_timer1_config->timer_mode_data.ctc_compare_value = TIMER1_COMPARE_VALUE_7SEC;
	TIMER_init(a_timer1_config);
	while(g_timer1_tick < 8);

	LCD_clearScreen(); /*clear the screen*/
	/*de-initialize timer 1*/
	g_timer1_tick = 0;
	TIMER_deInit(TIMER1_ID);
}

