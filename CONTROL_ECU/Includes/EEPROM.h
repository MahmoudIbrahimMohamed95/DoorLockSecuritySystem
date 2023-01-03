/*
 * EEPROM.h
 *
 *      Author: Mahmoud Ibrahim Mohamed
 *      August 2022
 *      updated December 2022
 */

#ifndef EEPROM_DRIVER_EEPROM_H_
#define EEPROM_DRIVER_EEPROM_H_

#include "std_types.h"


/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/
#define ERROR 0
#define SUCCESS 1

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void EEPROM_init(void);
uint8 EEPROM_writeByte(uint16 u16addr,uint8 u8data);
uint8 EEPROM_readByte(uint16 u16addr,uint8 *u8data);

#endif /* EXTERNAL_EEPROM_H_ */



