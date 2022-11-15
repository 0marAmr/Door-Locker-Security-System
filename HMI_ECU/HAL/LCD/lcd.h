 /******************************************************************************
 *
 * [FILE NAME]:     lcd.h
 *
 * [AUTHOR]:        Omar Amr
 *
 * [DATE]:          03-10-2022
 *
 * [Description]:   Header file for LCD driver
 *
 *******************************************************************************/

#ifndef LCD_H_
#define LCD_H_

#include "../../Utils/std_types.h"
#include "../../Utils/common_macros.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

/* LCD Data bits mode configuration, its value should be 4 or 8*/
#define LCD_DATA_BITS_MODE 8

/*start addresses of rows of LCD display*/
#define FIRST_ROW_START_ADDRESS 	0X00
#define SECOND_ROW_START_ADDRESS 	0X40
#define THIRD_ROW_START_ADDRESS 	0X10
#define FOURTH_ROW_START_ADDRESS 	0X50

#define BINARY_RADIX			2
#define DECIMAL_RADIX			10
#define HEXADECIMAL_RADIX		16
#define CHARACTER_FADE_DELAY	350

#if((LCD_DATA_BITS_MODE != 4) && (LCD_DATA_BITS_MODE != 8))

#error "Number of Data bits should be equal to 4 or 8"

#endif

/*LCD Hardware Ports and Pin definitions*/
#define LCD_RS_PORT_ID 		PORTD_ID 		/*REGISTER SELECT PORT ID*/
#define LCD_RS_PIN_ID 		PIN4_ID			/*REGISTER SELECT PIN ID */

#define LCD_E_PORT_ID		PORTD_ID		/*LCD ENABLE PORT ID*/
#define LCD_E_PIN_ID		PIN5_ID			/*LCD ENABLE PIN ID */

#define LCD_DATA_PORT_ID	PORTC_ID		/*LCD Bi-directional DATA bus PORT ID for DATA and COMMANDS*/

#if (LCD_DATA_BITS_MODE == 4)

/*The first pin in the nibble connected to the LCD */
#define LCD_DATA_PIN_1_ID 	PIN0_ID

#endif

/*LCD commands*/
#define LCD_DISPLAY_OFF							0x08
#define LCD_CLEAR_DISPLAY 						0x01
#define LCD_CURSOR_ON							0x0E
#define LCD_CURSOR_OFF							0x0C
#define LCD_CURSOR_BLINK						0x0F
#define LCD_CURSOR_GO_HOME						0x02
#define LCD_SET_CURSOR_LOCATION     			0x80
#define LCD_SHIFT_DISPLAY_LEFT					0x18
#define LCD_SHIFT_DISPLAY_RIGHT					0x1C
#define LCD_EIGHT_BITS_MODE						0x38
#define LCD_FOUR_BITS_MODE						0x28
#define LCD_TWO_LINES_FOUR_BITS_MODE_INIT1   	0x33
#define LCD_TWO_LINES_FOUR_BITS_MODE_INIT2   	0x32

/* Shifting the entire content of LCD screen*/
typedef enum {
	LEFT,RIGHT
}LCD_ShiftDirection;

#define MAX_SHIFT_LEFT_AMOUNT 24

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

/*
 * Description :
 * Send the required command to the screen
 */
void LCD_sendCommand(uint8 Command);

/*
 * Description :
 * Initialize the LCD:
 * 1. Setup the LCD pins directions by use the GPIO driver.
 * 2. Setup the LCD Data Mode 4-bits or 8-bits.
 */
void LCD_init(void);

/*
 * Description :
 * Display the required character on the screen
 */
void LCD_displayCharacter(uint8 Data);

/*
 * Description :
 * Display the required string on the screen
 */
void LCD_displayString(const uint8 * strConst);


/*
 * Description :
 * display a given data briefly, then replace it with a given symbol
 */
void LCD_characterFade(uint8 a_data, uint8 a_symbol);
/*
 * Description :
 * write the required string on the screen with delay effect
 */
void LCD_writeString(const uint8 * strConst, const uint16 a_writeDelay_ms);

/*
 * Description :
 * Move the cursor to a specified row and column index on the screen
 */
void LCD_moveCursor(uint8 row, uint8 col);

/*
 * Description :
 * Display the required string in a specified row and column index on the screen
 */
void LCD_displayStringRowColumn(uint8 row, uint8 col, const uint8 * str);

/*
 * Description :
 * Display the required decimal value on the screen
 */
void LCD_intgerToString(int data);

/*
 * Description :
 * Send the clear screen command
 */
void LCD_clearScreen(void);

/*
 * Description :
 * Shift the display in the specified direction each time delay time is elapsed
 * delay time is given in milliseconds
 */
void LCD_shiftDisplay(LCD_ShiftDirection direction,uint16 delay_time_ms);

#endif /* LCD_H_ */
