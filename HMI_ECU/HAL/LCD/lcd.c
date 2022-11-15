/******************************************************************************
 *
 * [FILE NAME]:     lcd.c
 *
 * [AUTHOR]:        Omar Amr
 *
 * [DATE]:          03-10-2022
 *
 * [Description]:   Source file for the LCD driver
 *
 *******************************************************************************/

#include "../../MCAL/GPIO/gpio.h"
#include "lcd.h"
#include <util/delay.h>
#include <stdlib.h>

static  uint8 g_rowNum = 0;
static  uint8 g_colNum = 0;

/*
 * Description :
 * Initialize the LCD:
 * 1. Setup the LCD pins directions by use the GPIO driver.
 * 2. Setup the LCD Data Mode 4-bits or 8-bits.
 */
void LCD_init(void){
	/*Configure RS pin and Enable pin as output pins */
	GPIO_setupPinDirection(LCD_RS_PORT_ID, LCD_RS_PIN_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_E_PORT_ID, LCD_E_PIN_ID, PIN_OUTPUT);

	_delay_ms(20); /* LCD Power ON delay > 15ms */

#if (LCD_DATA_BITS_MODE == 8)
	/*configure data/command port as output port */
	GPIO_setupPortDirection(LCD_DATA_PORT_ID, PORT_OUTPUT);

	/*configure LCD to work in 8-bit mode + 5*7 Dots Two Line Display Mode*/
	LCD_sendCommand(LCD_EIGHT_BITS_MODE);

#elif(LCD_DATA_BITS_MODE == 4)
	/*configure data/command port as output port */
	GPIO_setupNibbleDirection(LCD_DATA_PORT_ID, PORT_OUTPUT, LCD_DATA_PIN_1_ID);

	/*initialize 4-bit mode*/
	LCD_sendCommand(LCD_TWO_LINES_FOUR_BITS_MODE_INIT1);
	LCD_sendCommand(LCD_TWO_LINES_FOUR_BITS_MODE_INIT2);

	/*configure LCD to work in 4-bit mode + 5*7 Dots Two Line Display Mode*/
	LCD_sendCommand(LCD_FOUR_BITS_MODE);
#endif

	LCD_sendCommand(LCD_CURSOR_OFF);
	LCD_sendCommand(LCD_CLEAR_DISPLAY); /* clear LCD at the beginning */
}

/*
 * Description :
 * Send the required command to the screen
 */
void LCD_sendCommand(uint8 command){
	GPIO_writePin(LCD_RS_PORT_ID, LCD_RS_PIN_ID, LOGIC_LOW);
	_delay_ms(1);
	GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIN_ID, LOGIC_HIGH);
	_delay_ms(1);

#if (LCD_DATA_BITS_MODE ==8)
	GPIO_writePort(LCD_DATA_PORT_ID, command);

#elif (LCD_DATA_BITS_MODE ==4)

	GPIO_writeNibble(LCD_DATA_PORT_ID, GET_NIBBLE(command,MSN), LCD_DATA_PIN_1_ID);
	_delay_ms(1);

	GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIN_ID, LOGIC_LOW);
	_delay_ms(1);
	GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIN_ID, LOGIC_HIGH);
	_delay_ms(1);

	GPIO_writeNibble(LCD_DATA_PORT_ID, GET_NIBBLE(command,LSN), LCD_DATA_PIN_1_ID);

#endif
	_delay_ms(1);
	GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIN_ID, LOGIC_LOW);
	_delay_ms(1);
}

/*
 * Description :
 * Display the required character on the screen
 */
void LCD_displayCharacter(uint8 data){
	GPIO_writePin(LCD_RS_PORT_ID, LCD_RS_PIN_ID, LOGIC_HIGH);
	_delay_ms(1);
	GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIN_ID, LOGIC_HIGH);
	_delay_ms(1);

#if (LCD_DATA_BITS_MODE == 8)
	GPIO_writePort(LCD_DATA_PORT_ID, data);

#elif (LCD_DATA_BITS_MODE == 4)
	GPIO_writeNibble(LCD_DATA_PORT_ID,  GET_NIBBLE(data,MSN), LCD_DATA_PIN_1_ID);
	_delay_ms(1);

	GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIN_ID, LOGIC_LOW);
	_delay_ms(1);
	GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIN_ID, LOGIC_HIGH);
	_delay_ms(1);

	GPIO_writeNibble(LCD_DATA_PORT_ID,  GET_NIBBLE(data,LSN), LCD_DATA_PIN_1_ID);

#endif

	_delay_ms(1);
	GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIN_ID, LOGIC_LOW);
	_delay_ms(1);

	/*increment cursor position*/
	g_colNum++;
}

/*
 * Description :
 * Display the required string on the screen
 */
void LCD_displayString(const uint8 * strConst){
	uint8 i=0;
	for(i=0;strConst[i] != '\0';i++){
		LCD_displayCharacter(strConst[i]);
	}
}

/*
 * Description :
 * display a given data briefly, then replace it with a given symbol
 */
void LCD_characterFade(uint8 a_data, uint8 a_symbol){
	LCD_displayCharacter(a_data);
	g_colNum--;
	_delay_ms(CHARACTER_FADE_DELAY);
	LCD_moveCursor(g_rowNum, g_colNum);
	LCD_displayCharacter(a_symbol);
}
/*
 * Description :
 * write the required string on the screen with delay effect
 */
void LCD_writeString(const uint8 * strConst, const uint16 a_writeDelay_ms){
	uint8 i=0;
	for(i=0;strConst[i] != '\0';i++){
		LCD_displayCharacter(strConst[i]);
		_delay_ms(a_writeDelay_ms);
	}
}

/*
 * Description :
 * Move the cursor to a specified row and column index on the screen
 */
void LCD_moveCursor(uint8 row, uint8 col){
	uint8 lcd_location_address;

	/* Get the required address in the LCD DDRAM */
	switch(row){
	case 0:
		lcd_location_address = col + FIRST_ROW_START_ADDRESS;
		break;
	case 1:
		lcd_location_address = col + SECOND_ROW_START_ADDRESS;
		break;
	case 2:
		lcd_location_address = col + THIRD_ROW_START_ADDRESS;
		break;
	case 3:
		lcd_location_address = col + FOURTH_ROW_START_ADDRESS;
		break;
	}
	/* Move the LCD cursor to this specific address */
	LCD_sendCommand(LCD_SET_CURSOR_LOCATION | lcd_location_address);

	/*change cursor position*/
	g_rowNum = row;
	g_colNum = col;

}

/*
 * Description :
 * Display a given string in a specified row and column index on the screen
 */
void LCD_displayStringRowColumn(uint8 row, uint8 col, const uint8 * str){
	LCD_moveCursor(row, col); /* go to to the required LCD position */
	LCD_displayString(str);
}

/*
 * Description :
 * Display the required decimal value on the screen
 */
void LCD_intgerToString(int data){
	uint8 buffer[16];					/* String to hold the ascii result */
	itoa(data, buffer, DECIMAL_RADIX); 	/* Use itoa C function to convert the data to its corresponding ASCII value, 10 for decimal */
	LCD_displayString(buffer);
}

/*
 * Description :
 * Send the clear screen command
 */
void LCD_clearScreen(void){
	LCD_sendCommand(LCD_CLEAR_DISPLAY); /*send clear display command*/
}

/*
 * Description :
 * Shift the display in the specified direction each time delay time is elapsed
 * delay time is given in milliseconds
 */
void LCD_shiftDisplay(LCD_ShiftDirection direction, uint16 delay_time_ms){
	static volatile uint8 var =0;
	_delay_ms(delay_time_ms);

	/*check if the maximum left shift is reached*/
	if(var == MAX_SHIFT_LEFT_AMOUNT){
		var =0;
		LCD_sendCommand(LCD_CURSOR_GO_HOME); /*send cursor back to row 0 col 0*/
		return;
	}
	if(direction == RIGHT){
		LCD_sendCommand(LCD_SHIFT_DISPLAY_RIGHT);
	}
	else if(direction == LEFT){
		LCD_sendCommand(LCD_SHIFT_DISPLAY_LEFT);
		var++;

	}
}
