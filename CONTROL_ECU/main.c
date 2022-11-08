/******************************************************************************
 *
 * [FILE NAME]:     main.c
 *
 * [AUTHOR]:        Omar Amr
 *
 * [DATE]:          03-11-2022
 *
 * [Description]:   main function for CONTROL ECU
 *
 *******************************************************************************/

#include "APP/app.h"
#include "util/delay.h"

int main()
{
	/*variable to store command received from HMI ECU*/
	uint8 command;

	/********** Peripherals configurations **********/
	USART_ConfigType uart_config =
	{
			.usart_baud_rate = 9600,
			.usart_bit_mode = DATA_BITS_8,
			.usart_stop_bits = ONE_BIT,
			.usart_mode = ASYNCHRONOUS,
			.usart_parity = PARITY_DISABLED
	};

	/*configure timer0 to control the DC motor*/
	TIMER_ConfigType timer0_config =
	{
			.timer_id = TIMER0_ID,
			.prescaler.timer0 = TIMER0_F_CPU_64,
			.mode = PWM_MODE,
			.ocx_pin_behavior = CLEAR_OCX,
			.mode_data.pwm_duty_cycle = motor_speed_100,
	};

	TIMER_ConfigType timer1_config =
	{
			.timer_id = TIMER1_ID,
			.mode = COMPARE_MODE,
			.mode_data.ctc_compare_value = TIMER1_COMPARE_VALUE_7SEC,
			.prescaler.timer1 = TIMER1_F_CPU_1024,
			.ocx_pin_behavior = DISCONNECT_OCX,
	};

	/*Initialize the TWI/I2C Driver*/
	TWI_ConfigType twi_config =
	{
			.twi_slave_address = 0x01,
			.twi_prescaler = twi_prescaler_1,
			.twi_bit_rate = 400
	};

	/*set timer1 call back function*/
	TIMER_setCallBackFunc(TIMER1_ID, APP_timerTickIncrement);

	/*enable global interrupt bit (I-bit)*/
	sei();

	/*Peripherals & Modules Initialization*/
	DcMotor_init();
	BUZZER_init();
	TIMER_init(&timer0_config);
	TWI_init(&twi_config);
	USART_init(&uart_config);

	_delay_us(1); /*a small delay to initialize the peripherals*/

	/*Set a new password at the beginning of the program*/
	APP_changePassword();

	while(1){

		/*receive command from HMI ECU*/
		command = APP_receiveCommand();

		/*Execute the received command from HMI ECU*/
		switch(command){

		case OPEN_DOOR_COMMAND:
			APP_doorOpenSequence(&timer1_config);
			break;
		case CHANGE_PASSWORD_COMMAND:
			APP_changePassword();
			break;
		case ALARM_COMMAND:
			APP_alarmSequence(&timer1_config);
			break;
		case NO_COMMAND:
		default:
			; /*do nothing*/
		}
	}
}
