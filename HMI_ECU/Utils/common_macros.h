/******************************************************************************
 *
 * [FILE NAME]:     common_macros.h
 *
 * [AUTHOR]:        Omar Amr
 *
 * [DATE]:          01-10-2022
 *
 * [Description]:   Commonly used Macros
 *
 *******************************************************************************/

#ifndef COMMON_MACHROS_H_
#define COMMON_MACHROS_H_

/* Set a certain bit in any register */
#define SET_BIT(REG,BIT){\
		(REG) = ((REG) | (1<<(BIT)));\
}\

/* Clear a certain bit in any register */
#define CLEAR_BIT(REG,BIT){\
		(REG) = ((REG) & ~(1<<(BIT)));\
}\

/* Toggle a certain bit in any register */
#define TOGGLE_BIT(REG,BIT){\
		(REG) = ((REG) ^ (1<<BIT));\
}\

/* Get the value of a specific bit in a given register*/
#define GET_BIT(REG,BIT)(((REG)&(1<<(BIT)))>>(BIT))

/* Rotate left the register value with specific number of rotates */
#define ROL(REG,BIT){\
		(REG) = ((REG)<<(BIT)) | ((REG)>>(REG_SIZE-BIT));\
}\

/* Rotate right the register value with specific number of rotates */
#define  ROR(REG,BIT){\
		(REG) = ((REG)>>(BIT)) | ((REG)<<(REG_SIZE-BIT));\
}\

/* Check if a specific bit is set in any register and return true if yes */
#define BIT_IS_SET(REG,BIT) ( REG & (1<<BIT) )

/* Check if a specific bit is cleared in any register and return true if yes */
#define BIT_IS_CLEAR(REG,BIT) ( !(REG & (1<<BIT)) )

/*Set 4 consecutive bits starting from a certain bit position (SIG) in any register  */
#define SET_NIBBLE(REG,SIG){\
		(REG) = (REG) | (0x0F<<SIG);\
}\

/*Clear 4 consecutive bits starting from a certain bit position (SIG) in any register  */
#define CLEAR_NIBBLE(REG,SIG){\
		(REG) = ( (REG) & ((REG) & (((0xF0 << (SIG))| (0xF0u >> (8 - SIG))))) );\
}\

/*Write a 4-bit value in 4 consecutive bits starting from a certain bit position (SIG) in any register  */
#define WRTIE_NIBBLE(REG,VALUE,SIG){\
		(REG) = (((REG) & (((0xF0 << (SIG))| (0xF0u >> (8 - SIG))))) | ((VALUE<<SIG) & (0x0F<<SIG)));\
}\

/*Get the value stored in a 4 consecutive bits starting from a certain bit position (POS) */
#define GET_NIBBLE(REG,POS) ( ( REG & (0x0Fu<<POS) ) >> POS )

#endif /*COMMON_MACHROS_H_*/
