/******************************************************************************
 *
 * [FILE NAME]:     gpio.h
 *
 * [AUTHOR]:        Omar Amr
 *
 * [DATE]:          01-10-2022
 *
 * [Description]:   Header file for General IO Driver
 * 
 * [TARGET HW]:		ATmega32
 *
 *******************************************************************************/

#ifndef GPIO_H_
#define GPIO_H_

#include "../../Utils/std_types.h"
#include "../../Utils/common_macros.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define NUM_OF_PORTS           4
#define NUM_OF_PINS_PER_PORT   8

#define PORTA_ID               0
#define PORTB_ID               1
#define PORTC_ID               2
#define PORTD_ID               3

#define PIN0_ID                0
#define PIN1_ID                1
#define PIN2_ID                2
#define PIN3_ID                3
#define PIN4_ID                4
#define PIN5_ID                5
#define PIN6_ID                6
#define PIN7_ID                7

/******** PortA Registers *******/
#define PORTA 	(*((volatile uint8 * const)0x3B))
#define DDRA 	(*((volatile uint8 * const)0x3A))
#define PINA 	(*((const volatile uint8 *)0x39))

/******** PortB Registers *******/
#define PORTB 	(*((volatile uint8 * const)0x38))
#define DDRB 	(*((volatile uint8 * const)0x37))
#define PINB 	(*((const volatile uint8 * const)0x36))

/******** PortC Registers *******/
#define PORTC 	(*((volatile uint8 * const)0x35))
#define DDRC 	(*((volatile uint8 * const)0x34))
#define PINC 	(*((const volatile uint8 * const)0x33))

/******** PortD Registers *******/
#define PORTD 	(*((volatile uint8 * const)0x32))
#define DDRD 	(*((volatile uint8 * const)0x31))
#define PIND 	(*((const volatile uint8 *)0x30))

/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/
typedef enum{
	PIN_INPUT,PIN_OUTPUT
}GPIO_PinDirectionType;

typedef enum{
	NIBBLE_INPUT, NIBBLE_OUTPUT
}GPIO_NibbleDirectionType;

typedef enum{
	PORT_INPUT,PORT_OUTPUT=0xFF
}GPIO_PortDirectionType;

typedef enum{
	/*LEAST SIGNIFICANT NIBBLE, MOST SIGNIFICANT NIBBLE*/
	LSN, MSN =4
}GPIO_NibbleSignificance;

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

/*
 * Description :
 * Setup the direction of the required pin input/output.
 * If the input port number or pin number are not correct, The function will not handle the request.
 */
void GPIO_setupPinDirection(uint8 port_num, uint8 pin_num, GPIO_PinDirectionType direction);

/*
 * Description :
 * Write the value Logic High or Logic Low on the required pin.
 * If the input port number or pin number are not correct, The function will not handle the request.
 * If the pin is input, this function will enable/disable the internal pull-up resistor.
 */
void GPIO_writePin(uint8 port_num, uint8 pin_num, uint8 value);

/*
 * Description :
 * Read and return the value for the required pin, it should be Logic High or Logic Low.
 * If the input port number or pin number are not correct, The function will return Logic Low.
 */
uint8 GPIO_readPin(uint8 port_num, uint8 pin_num);

/*
 * Description :
 * Setup the direction of the required port all pins input/output.
 * If the direction value is PORT_INPUT all pins in this port should be input pins.
 * If the direction value is PORT_OUTPUT all pins in this port should be output pins.
 * If the input port number is not correct, The function will not handle the request.
 */
void GPIO_setupPortDirection(uint8 port_num, GPIO_PinDirectionType direction);

/*
 * Description :
 * Write the value on the required port.
 * If any pin in the port is output pin the value will be written.
 * If any pin in the port is input pin this will activate/deactivate the internal pull-up resistor.
 * If the input port number is not correct, The function will not handle the request.
 */
void GPIO_writePort(uint8 port_num, uint8 value);

/*
 * Description :
 * Read and return the value of the required port.
 * If the input port number is not correct, The function will return ZERO value.
 */
uint8 GPIO_readPort(uint8 port_num);

/*
 * Description :
 * Setup the direction of the required nibble of a port as input/output.
 * If the direction value is PORT_INPUT all pins in the nibble of the port should be input pins.
 * If the direction value is PORT_OUTPUT all pins n the nibble of the port should be output pins.
 * The Nibble of a Port is chosen to the be most or the least significant nibble.
 * If the input port number is not correct, The function will not handle the request.
 */
void GPIO_setupNibbleDirection(uint8 port_num, GPIO_NibbleDirectionType direction, GPIO_NibbleSignificance nibble_choice);

/*
 * Description :
 * Read and return the value of the required nibble.
 * If the input port number is not correct, The function will return ZERO value.
 */
uint8 GPIO_readNibble(uint8 port_num, GPIO_NibbleSignificance nibble_choice );

/*
 * Description :
 * Write the value on the required port.
 * If any pin in the port is output pin the value will be written.
 * If any pin in the port is input pin this will activate/deactivate the internal pull-up resistor.
 * If the input port number is not correct, The function will not handle the request.
 */
void GPIO_writeNibble(uint8 port_num, uint8 value, GPIO_NibbleSignificance nibble_choice);

#endif /* GPIO_H_ */
