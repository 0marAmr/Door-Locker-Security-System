/******************************************************************************
 *
 * [FILE NAME]:     usart.h
 *
 * [AUTHOR]:        Omar Amr
 *
 * [DATE]:          9-10-2022
 *
 * [Description]:   Source file for Universal Synchronous Asynchronous  Receiver Transmitter
 *
 * [TARGET HW]:		ATmega32
 *
 *******************************************************************************/

#ifndef USART_H_
#define USART_H_

#include "../../Utils/std_types.h"
#include "../../Utils/common_macros.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define USART_TERMINATOR_CHARACTER 		'#'  /*A special character denoting the end of a string*/
#define USART_RECEIVER_READY_BYTE		0x66 /*Byte sent from receiver when it is ready to receive*/
#define USART_SENDER_READY_BYTE		0x66 /*Byte sent from receiver when it is ready to receive*/

/*Mapped Peripheral registers addresses definitions*/
#define UCSRA (*( (volatile uint8 * const) 	0x2B))
#define UCSRB (*( (volatile uint8 * const) 	0x2A))
#define UCSRC (*( (volatile uint8 * const) 	0x40))
#define UBRRH UCSRC
#define UBRRL (*( (volatile uint8 * const) 	0x29))
#define UDR   (*( (volatile uint8 * const) 	0x2C))

/*UCSRA*/
#define MPCM    0
#define U2X     1
#define PE      2
#define DOR     3
#define FE      4
#define UDRE    5
#define TXC     6
#define RXC     7

/*UCSRB*/
#define TXB8    0
#define RXB8    1
#define UCSZ2   2
#define TXEN    3
#define RXEN    4
#define UDRIE   5
#define TXCIE   6
#define RXCIE   7

/*UCSRC*/
#define UCPOL   0
#define UCSZ0   1
#define UCSZ1   2
#define USBS    3
#define UPM0    4
#define UPM1    5
#define UMSEL   6
#define URSEL   7

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/

typedef enum{
	PARITY_DISABLED, EVEN_PARITY = 2, ODD_PARITY
}USART_ParityType;

typedef enum{
	ASYNCHRONOUS, SYNCHRONOUS
}USART_ModeSelect;

typedef enum{
	ONE_BIT, TWO_BITS
}USART_StopBitsSelect;

typedef enum{
	DATA_BITS_5, DATA_BITS_6, DATA_BITS_7, DATA_BITS_8, DATA_BITS_9 = 7
}USART_BitMode;

typedef enum{
	TX_RISING_RX_FALLING, TX_FALLING_RX_RISING
}USART_ClockPolarity;

typedef struct{
	uint32 usart_baud_rate;
	USART_BitMode usart_bit_mode;
	USART_StopBitsSelect usart_stop_bits;
	USART_ModeSelect usart_mode;
	USART_ParityType usart_parity;
	USART_ClockPolarity usart_clock_config;
}USART_ConfigType;

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the USART.
 * 3. Setup the USART baud rate.
 */
void USART_init(const USART_ConfigType * const a_usartConfig);

/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void USART_sendByte(uint8 a_data);

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 USART_receiveByte(void);

/*
 * Description :
 * Receive the required string until the terminator symbol.
 */
void USART_receiveString(uint8 * const a_rxStrPtr);

#endif /*USART_H_*/
