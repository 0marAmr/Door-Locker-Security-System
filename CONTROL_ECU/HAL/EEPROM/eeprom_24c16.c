/******************************************************************************
 *
 * [FILE NAME]:     eeprom_24c16.c
 *
 * [AUTHOR]:        Omar Amr
 *
 * [DATE]:          01-11-2022
 *
 * [Description]:   Source file for EEPROM 24C16
 *
 *******************************************************************************/

#include "../../MCAL/I2C/twi.h"
#include "eeprom_24c16.h"

/*******************************************************************************
 *                     		 Functions Definitions                             *
 *******************************************************************************/

uint8 EEPROM_writeByte(uint16 u16addr,uint8 u8data)
{
	/* Send the Start Bit */
	TWI_start();
	if(TWI_getStatus() != TWI_START)
	{
		return ERROR;
	}

	/* Send the device address, we need to get A8 A9 A10 address bits from the
	 * memory location address and R/W=0 (write) */
	TWI_writeByte((uint8)(((u16addr & 0x0700)>>7) | (0xA0)));
	if(TWI_getStatus() != TWI_MT_SLA_W_ACK)
	{
		return ERROR;
	}

	/* Send the required memory location address */
	TWI_writeByte((uint8)(u16addr & 0x00FF));
	if(TWI_getStatus() != TWI_MT_DATA_ACK)
	{
		return ERROR;
	}

	/* write byte to eeprom */
	TWI_writeByte(u8data);
	if(TWI_getStatus() != TWI_MT_DATA_ACK)
	{
		return ERROR;
	}

	/* Send the Stop Bit */
	TWI_stop();

	return SUCCESS;
}

uint8 EEPROM_readByte(uint16 u16addr,uint8 *u8data)
{
	/* Send the Start Bit */
	TWI_start();
	if(TWI_getStatus() != TWI_START)
	{
		return ERROR;
	}

	/* Send the device address, we need to get A8 A9 A10 address bits from the
	 * memory location address and R/W=0 (write) */
	TWI_writeByte((uint8)(((u16addr & 0x0700)>>7) | (0xA0)));
	if(TWI_getStatus() != TWI_MT_SLA_W_ACK)
	{
		return ERROR;
	}

	/* Send the required memory location address */
	TWI_writeByte((uint8)(u16addr & 0x00FF));
	if(TWI_getStatus() != TWI_MT_DATA_ACK)
	{
		return ERROR;
	}

	/* Send the Repeated Start Bit */
	TWI_start();
	if (TWI_getStatus() != TWI_REP_START)
	{
		return ERROR;
	}

	/* Send the device address, we need to get A8 A9 A10 address bits from the
	 * memory location address and R/W=1 (Read) */
	TWI_writeByte((uint8)(((u16addr & 0x0700)>>7) | (0xA1)));
	if(TWI_getStatus() != TWI_MT_SLA_R_ACK)
	{
		return ERROR;
	}

	/* Read Byte from Memory without send ACK */
	*u8data = TWI_readByteWithNACK();
	if (TWI_getStatus() != TWI_MR_DATA_NACK)
	{
		return ERROR;
	}

	/* Send the Stop Bit */
	TWI_stop();

	return SUCCESS;
}
