/******************************************************************************
 *
 * [FILE NAME]:     usart.c
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

#include "usart.h"

/*******************************************************************************
 *                     		 Functions Definitions                             *
 *******************************************************************************/

/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the USART.
 * 3. Setup the USART baud rate.
 */
void USART_init(const USART_ConfigType * const a_usartConfigPtr){
	uint16 reg_UBRR_value = 0;

	/* U2X = 1 for double transmission speed */
	UCSRA = (1<<U2X);

	/************************** UCSRB Description **************************
	 * RXCIE = 0 Disable USART RX Complete Interrupt Enable
	 * TXCIE = 0 Disable USART Tx Complete Interrupt Enable
	 * UDRIE = 0 Disable USART Data Register Empty Interrupt Enable
	 * TXEN  = 1 Transmitter Enable
	 * RXEN  = 1 Receiver Enable
	 * UCSZ2 = 1/0 For 9/other data bit mode
	 * RXB8 & TXB8 not used for 8-bit data mode
	 ***********************************************************************/
	UCSRB = ((a_usartConfigPtr->usart_bit_mode & 0x04)) | (1<<TXEN) | (1<<RXEN);

	/************************** UCSRC Description **************************
	 * URSEL   = 1 The URSEL must be one when writing the UCSRC
	 * UMSEL   = 0/1 Async/Sync Operation
	 * UPM1:0   (parity bit configuration)
	 * USBS    = 0/1 One/Two stop bit(s)
	 * UCSZ1:0  (data bits mode config.)
	 ***********************************************************************/
	UCSRC = (1 << URSEL) | (a_usartConfigPtr->usart_mode << UMSEL) | (a_usartConfigPtr->usart_parity << UPM0)\
			| ( a_usartConfigPtr->usart_stop_bits << USBS) | ((a_usartConfigPtr->usart_bit_mode & 0x03) << UCSZ0);

	if(a_usartConfigPtr->usart_mode == SYNCHRONOUS){
		/* UCPOL   	(clock configuration for Async. mode)*/
		UCSRC |= (a_usartConfigPtr->usart_clock_config << UCPOL);
	}

	/* Calculate the UBRR register value */
	reg_UBRR_value = (uint16)( ( F_CPU / (8UL * a_usartConfigPtr->usart_baud_rate) ) - 1 );

	/*Clear URSEL to write in UBRRH Register*/
	CLEAR_BIT(UBRRH,URSEL);
	/* First 8 bits from the BAUD_PRESCALE inside UBRRL and last 4 bits in UBRRH*/
	UBRRH = (uint8)   (reg_UBRR_value >> 8) ;
	UBRRL = (uint8)   (reg_UBRR_value & 0x00FF);
}

/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void USART_sendByte(uint8 a_data){

	/*Wait until the uart transmitter buffer is ready to recieve a new data*/
	while(BIT_IS_CLEAR(UCSRA,UDRE));

	/*Write data to UDR register (in transmission buffer) to be sent*/
	UDR = a_data;
}

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 USART_receiveByte(void){
	/*Wait until data is recieved and the RXC flag is raised*/
	while(BIT_IS_CLEAR(UCSRA,RXC));

	/*return data in the recieve buffer*/
	return UDR; /*RXC is cleared after reading*/
}

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void USART_sendString(const uint8 * a_txStrPtr){
	uint8 i=0;

	/* Send the string (without null terminator char) */
	while(a_txStrPtr[i] != '\0'){
		USART_sendByte(a_txStrPtr[i]);
		i++;
	}
}

/*
 * Description :
 * Receive the required string until the terminator symbol.
 */
void USART_receiveString(uint8 * const a_rxStrPtr){
	uint8 i=0;

	/* Receive the whole string until the defined terminator char */
	do{
		a_rxStrPtr[i] = USART_receiveByte();
	}
	while(a_rxStrPtr[i++] != USART_TERMINATOR_CHARACTER);

	/*replacing  the retminator character with a null terminator*/
	a_rxStrPtr[i-1] = '\0';
}
