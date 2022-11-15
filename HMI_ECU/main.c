/******************************************************************************
 *
 * [FILE NAME]:     main.c
 *
 * [AUTHOR]:        Omar Amr
 *
 * [DATE]:          03-11-2022
 *
 * [Description]:   main function for HMI ECU
 *
 *******************************************************************************/

#include "APP/app.h"

int main(void)
{
	/*variable to store the user choice */
	uint8 choice = 0;

	/********** Peripherals configurations **********/
	USART_ConfigType uart_config =
	{
			.usart_baud_rate = 9600,
			.usart_bit_mode = DATA_BITS_8,
			.usart_stop_bits = ONE_BIT,
			.usart_mode = ASYNCHRONOUS,
			.usart_parity = PARITY_DISABLED
	};

	TIMER_ConfigType timer1_config =
	{
			.timer_id = TIMER1_ID,
			.timer_mode = COMPARE_MODE,
			.timer_mode_data.ctc_compare_value = TIMER1_COMPARE_VALUE_7SEC,
			.timer_prescaler.timer1 = TIMER1_F_CPU_1024,
			.timer_ocx_pin_behavior = DISCONNECT_OCX,
	};

	TIMER_setCallBackFunc(TIMER1_ID, APP_timerTickIncrement);	/*set timer1 call back function*/

	sei(); 		/*enable global interrupt bit (I-bit)*/

	/*Peripherals & Modules Initialization*/
	USART_init(&uart_config);
	LCD_init();

	/*Display welcome message at program start.*/
	APP_welcomeScreen();
	/*Set a new password at the beginning of the program*/
	APP_setNewPassword();

	while(1)
	{
		/*Get user choice*/
		choice = APP_mainMenu();

		switch (choice)
		{
		case DOOR_OPEN:
			APP_doorOpenSequence(&timer1_config);
			break;
		case CHANGE_PASS:
			APP_changePasswordSequence();
			break;
		case ALARM :
			APP_alarmSequence(&timer1_config);
			break;
		default:
			;	/*do nothing*/
		}
	}
}
