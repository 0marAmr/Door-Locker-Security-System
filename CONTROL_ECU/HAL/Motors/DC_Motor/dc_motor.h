/******************************************************************************
 *
 * [FILE NAME]:     dc_motor.h
 *
 * [AUTHOR]:        Omar Amr
 *
 * [DATE]:          10-10-2022
 *
 * [DESCRIPTION]:   Header file for DC Motor Driver
 *
 *******************************************************************************/

#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

#include "../../../Utils/std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define DC_MOTOR_IN1_PORT_ID	PORTB_ID
#define DC_MOTOR_IN1_PIN_ID		PIN0_ID

#define DC_MOTOR_IN2_PORT_ID	PORTB_ID
#define DC_MOTOR_IN2_PIN_ID		PIN1_ID

#define DC_MOTOR_EN_PORT_ID 	PORTB_ID
#define DC_MOTOR_EN_PIN_ID 		PIN3_ID

 enum{
	motor_speed_0 	= 0,
	motor_speed_25 	= 25,
	motor_speed_50	= 50,
	motor_speed_75	= 75,
	motor_speed_100	= 100
};

/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/

typedef enum{
	CW, ACW
}DcMotor_State;


/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

/*Description:
 * Function that turns off the Dc Motor
 * */
void DcMotor_off(void);

/*Description:
 * Function to Initialize the motor:
 * 1. Setup the direction for the two motor pins.
 * 2. Stop at the DC-Motor at the beginning
 * */
void DcMotor_init();

/*Description:
 * Rotate the DC Motor CW/ or A-CW or stop the motor.
 * Set the required duty cycle via PWM driver according to speed value.
 * */
void DcMotor_rotate(DcMotor_State state, uint8 speed, TIMER_ConfigType * a_timer0_configPtr);


#endif /* DC_MOTOR_H_ */
