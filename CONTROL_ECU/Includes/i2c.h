/*
 * i2c.h
 *
 *  Created on: Aug 26, 2022
 *      Author: K I N G
 */

#ifndef I2C_DRIVER_I2C_H_
#define I2C_DRIVER_I2C_H_


#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"

/*******************************************************************************
 *                              Types Declaration
 *******************************************************************************/
typedef enum{
	one , four , sixteen , sixty_four
}Prescaler;

typedef struct{

	uint8 MY_SLAVE_ADDRESS ;
	uint8 BIT_RATE ;
	Prescaler prescaler;
}i2c_Config;

/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/
/* I2C Status Bits in the TWSR Register */
#define TW_START         0x08 // start has been sent
#define TW_REP_START     0x10 // repeated start
#define TW_MT_SLA_W_ACK  0x18 // Master transmit ( slave address + Write request ) to slave + Ack received from slave
#define TW_MT_SLA_R_ACK  0x40 // Master transmit ( slave address + Read request ) to slave + Ack received from slave
#define TW_MT_DATA_ACK   0x28 // Master transmit data and ACK has been received from Slave.
#define TW_MR_DATA_ACK   0x50 // Master received data and send ACK to slave
#define TW_MR_DATA_NACK  0x58 // Master received data but doesn't send ACK to slave

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void TWI_init(i2c_Config*Config_ptr);
void TWI_start(void);
void TWI_stop(void);
void TWI_write(uint8 data);
uint8 TWI_readWithACK(void); //read with send Ack
uint8 TWI_readWithNACK(void); //read without send Ack
uint8 TWI_getStatus(void);

#endif /* I2C_DRIVER_I2C_H_ */
