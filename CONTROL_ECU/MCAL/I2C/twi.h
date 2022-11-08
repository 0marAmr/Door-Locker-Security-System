/******************************************************************************
 *
 * [FILE NAME]:     twi.h
 *
 * [AUTHOR]:        Omar Amr
 *
 * [DATE]:          01-11-2022
 *
 * [Description]:   Header file for two wire interface (TWI/I2C) driver
 *
 *******************************************************************************/

#ifndef MCAL_I2C_TWI_H_
#define MCAL_I2C_TWI_H_

#include "../../Utils/std_types.h"
#include "../../Utils/common_macros.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

/* I2C Status Bits in the TWSR Register */
#define TWI_START         0x08 /* start has been sent */
#define TWI_REP_START     0x10 /* repeated start */
#define TWI_MT_SLA_W_ACK  0x18 /* Master transmit ( slave address + Write request ) to slave + ACK received from slave. */
#define TWI_MT_SLA_R_ACK  0x40 /* Master transmit ( slave address + Read request ) to slave + ACK received from slave. */
#define TWI_MT_DATA_ACK   0x28 /* Master transmit data and ACK has been received from Slave. */
#define TWI_MR_DATA_ACK   0x50 /* Master received data and send ACK to slave. */
#define TWI_MR_DATA_NACK  0x58 /* Master received data but doesn't send ACK to slave. */

/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/

typedef  uint8 TWI_Address;
typedef  uint8 TWI_BitRate;
typedef enum{
	twi_prescaler_1, twi_prescaler_4, twi_prescaler_16, twi_prescaler_64
}TWI_Prescaler;

typedef struct {
	TWI_Address twi_slave_address;
	TWI_BitRate twi_bit_rate;
	TWI_Prescaler twi_prescaler;
}TWI_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

void TWI_init(TWI_ConfigType * a_twiConfig);
void TWI_start(void);
void TWI_stop(void);
void TWI_writeByte(uint8 data);
uint8 TWI_readByteWithACK(void);
uint8 TWI_readByteWithNACK(void);
uint8 TWI_getStatus(void);

#endif /* MCAL_I2C_TWI_H_ */
