/******************************************************************************
 *
 * [FILE NAME]:     app.c
 *
 * [AUTHOR]:        Omar Amr
 *
 * [DATE]:          02-11-2022
 *
 * [Description]:   Source file for Door lock Application (CONTROL ECU)
 *
 *******************************************************************************/

#include "app.h"

/*******************************************************************************
 *                     	   	  Global Variables                                 *
 *******************************************************************************/

uint8 g_receivedPassword[6] = {0}; /*User password input received from HMI ECU*/
/*
 * A buffer that stores:
 * 1- the received password confirmation.
 * 2- the password retrieved from the EEPROM
 * in any case, the received password is compared with the buffer value.
 * */
uint8 g_passwordBuffer[6] = {0};
uint8 g_wrong_passwords = 0;	/*wrong passwords counter*/
uint8 g_timer1_tick = 0;		/*timer 1 compare match counter*/


/*******************************************************************************
 *                      Functions Prototypes(Private)                          *
 *******************************************************************************/

/*
 * Description:
 * A function that receive a password string from the HMI ECU.
 * The password is stored in a given password buffer.
 */
static void APP_receivePassword(uint8 * const a_password);

/*
 * Description:
 * A generic function that checks whether the entered password matches:
 * 1- The re-entered password in case of password change/initialization
 * 2- The actual password stored in the EEPROM in case of a door access or password change.
 * the function sends back the status of the password to HMI ECU
 */
static APP_PasswordStatus APP_confirmPassword(const uint8 * const a_password1, const uint8 * const a_password2);

/*
 * Description:
 * A  function that receives the new password & it's confirmation from user.
 * 1- The function confirms if the two received passwords match each other.
 * 2- In case of matching, it stores the password in the EEPROM.
 * 3- It return status in both of matching and non-matching cases.
 */
static APP_PasswordStatus APP_newPasswordConfirm(void);

/*
 * Description:
 * Save the received  password in EEPROM memory.
 * */
static void APP_savePassword(void);

/*
 * Description:
 * retrieve the password from EEPROM memory and store it in a global buffer.
 * */
static void APP_retrievePassword(void);

/*******************************************************************************
 *                     		 Functions Definitions                             *
 *******************************************************************************/

/*
 * Description:
 * A function that receive a password string from the HMI ECU.
 * The password is stored in a given password buffer.
 */
static void APP_receivePassword(uint8 * const a_password)
{

	/*Acknowledge that receiver (CONTROL ECU) is ready to receive password value */
	USART_sendByte(USART_RECEIVER_READY_BYTE);

	while( USART_receiveByte() != USART_SENDER_READY_BYTE && USART_receiveByte() != ALARM_COMMAND )
	{
		; /*wait until the sender is (HMI ECU) ready to send the password*/
	}

	USART_receiveString(a_password); /*receive the password*/
}

/*
 * Description:
 * A generic function that checks whether the entered password matches:
 * 1- The re-entered password in case of password change/initialization
 * 2- The actual password stored in the EEPROM in case of a door access or password change.
 * the function sends back the status of the password to HMI ECU
 */
static APP_PasswordStatus APP_confirmPassword(const uint8 * const a_password1, const uint8 * const a_password2)
{
	uint8 i = 0;
	/*passwords are matching unless otherwise is proved*/
	APP_PasswordStatus status = MATCHING_PASSWORDS;

	for (i = 0; i < PASSWORD_LENGTH; i++)
	{
		if(g_receivedPassword[i] != g_passwordBuffer[i])
		{
			status =  UNMATCHING_PASSWORDS;
			break;
		}
	}

	/*Acknowledge that sender (CONTROL ECU) is ready */
	USART_sendByte(USART_SENDER_READY_BYTE);

	while( USART_receiveByte() != USART_RECEIVER_READY_BYTE){
		; /*wait until the receiver (HMI ECU) is ready*/
	}

	if(status == MATCHING_PASSWORDS)
	{
		USART_sendByte(MATCHING_PASSWORD_BYTE);
	}
	else
	{
		USART_sendByte(UNMATCHING_PASSWORD_BYTE);
	}

	return status ;
}

/*
 * Description:
 * A  function that receives the new password & it's confirmation from user.
 * 1- The function confirms if the two received passwords match each other.
 * 2- In case of matching, it stores the password in the EEPROM.
 * 3- It return status in both of matching and non-matching cases.
 */
static APP_PasswordStatus APP_newPasswordConfirm(void)
{
	/*receive the password and it's confirmation and store it*/
	APP_receivePassword(g_receivedPassword);	/*the password*/
	APP_receivePassword(g_passwordBuffer);	/*the password confirmation*/

	/*compare the two passwords*/
	if(APP_confirmPassword(g_receivedPassword,g_passwordBuffer) == MATCHING_PASSWORDS)
	{
		APP_savePassword(); /*save the password in EEPROM*/
		return MATCHING_PASSWORDS;
	}
	else
	{
		return UNMATCHING_PASSWORDS;
	}
}

/*
 * Description:
 * Save the received  password in EEPROM memory.
 * */
static void APP_savePassword(void){
	uint8 i = 0;
	for( i =0; i<PASSWORD_LENGTH; i++){
		EEPROM_writeByte(PASSWORD_BASE_ADDRESS+i, g_receivedPassword[i]);
	}
}

static void APP_retrievePassword(void){
	uint8 i = 0;
	for( i =0; i<PASSWORD_LENGTH; i++){
		EEPROM_readByte(PASSWORD_BASE_ADDRESS + i, g_passwordBuffer + i);
	}
}

/*
 * Description:
 * Sequence of steps that CONTROL_ECU does when opening the door:
 * 1- rotates the motor CW for 15 seconds 	: Opens the Door
 * 2- turn off the motor for 3 seconds		: Hold the Door open
 * 3- rotates the motor ACW for 15 seconds 	: Closes the Door
 * */
void APP_doorOpenSequence(TIMER_ConfigType * const a_timer1_configPtr)
{
	/*rotate the motor CW for 15 seconds*/
	DcMotor_rotate(CW, motor_speed_100, a_timer1_configPtr);
	TIMER_init(a_timer1_configPtr);	/*initialize and configure timer 1*/
	while(g_timer1_tick < 2);

	/*turn off the motor for 15 seconds*/
	DcMotor_off();
	TIMER_changeCompareValue(TIMER1_ID, TIMER1_COMPARE_VALUE_3SEC);
	while(g_timer1_tick < 3);

	/*rotate the motor ACW for 15 seconds*/
	DcMotor_rotate(ACW, motor_speed_100, a_timer1_configPtr);
	TIMER_changeCompareValue(TIMER1_ID, TIMER1_COMPARE_VALUE_7SEC);
	while(g_timer1_tick < 5);

	/* turn off the timer and the motor*/
	TIMER_deInit(TIMER1_ID);
	DcMotor_off();
	g_timer1_tick = 0;
}

/*
 * Description:
 * A function that changes the password.
 * The function is executed in case of a New password or changing an existing one.
 * */
void APP_changePassword(void)
{
	while (APP_newPasswordConfirm() == UNMATCHING_PASSWORDS)
	{
		; /*keep waiting until the two received passwords match*/
	}
}

/*
 * Description:
 * sets the buzzer for 1 minute.
 * */
void APP_alarmSequence(TIMER_ConfigType * const a_timer1_config)
{
	/*starts the buzzer*/
	BUZZER_start();
	/*configure timer1 to count for a minute*/
	a_timer1_config->mode_data.ctc_compare_value = TIMER1_COMPARE_VALUE_7SEC;
	TIMER_init(a_timer1_config);
	while(g_timer1_tick < 8);

	BUZZER_stop(); /*stop the buzzer*/

	/* de-initialize timer 1*/
	g_timer1_tick = 0;
	TIMER_deInit(TIMER1_ID);
}

/*
 * Description :
 * Callback function  that increments a global variable g_timer1_tick
 */
void APP_timerTickIncrement(void){
	g_timer1_tick++;
}

/*
 * Description:
 * Function that receives a command from HMI ECU.
 * It returns the given command or NO_COMMAND if non is received (wrong pass).
 * */
APP_Commands APP_receiveCommand(void){
	/*receive password from HMI ECU to perform actions*/
	APP_receivePassword(g_receivedPassword);

	/*get the password form memory and store it in a buffer to be compared with the received one*/
	APP_retrievePassword();
	if(APP_confirmPassword(g_receivedPassword,g_passwordBuffer) == MATCHING_PASSWORDS)
	{
		/*reset the counter if a correct password is entered*/
		g_wrong_passwords = 0;

		/*Acknowledge that receiver (CONTROL ECU) is ready to receive command*/
		USART_sendByte(USART_RECEIVER_READY_BYTE);

		while( USART_receiveByte() != USART_SENDER_READY_BYTE)
		{
			; /*wait until the sender is (HMI ECU) ready to send the password*/
		}
		return USART_receiveByte();
	}
	else
	{
		g_wrong_passwords++;
		if(g_wrong_passwords >= MAX_WRONG_PASSWORDS) /*the user has up to 3 trails*/
		{
			return ALARM_COMMAND;
		}
		else
		{
			return NO_COMMAND; /*no command is received for HMI ECU*/
		}
	}
}
