/*
 * uart.h
 *
 *
 *      Author: Mahmoud Ibrahim
 *      august 2022
 *      updated December 2022
 */

#ifndef UART_H_
#define UART_H_

#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"


/*******************************************************************************
 *                      Types Declaration                                      *
 *******************************************************************************/

typedef enum{
	one_bit , two_bit
}STOP_BIT_NUMBER;


typedef enum{
	disabled , reserved , even_parity , odd_parity
}PARITY_TYBE;


typedef enum{
	five_bits , six_bit , seven_bits , eight_bits , reserved1 , reserved2 , reserved3 ,nine_bits
}DATA_BITS;


typedef enum{
	Normal_Speed , Double_Speed
}TRANSMISSION_SPEED;

typedef struct{

	TRANSMISSION_SPEED   TRANSMISSION_SPEED;

	PARITY_TYBE          PARITY    ;

	uint16               BAUD_RATE ;

	STOP_BIT_NUMBER      STOP_BIT  ;



	DATA_BITS            DATA_BITS ;

}UART_Config;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void UART_init(UART_Config* Config_ptr);

void UART_sendByte(const uint8 data);

uint8 UART_recieveByte(void);

void UART_sendString(const uint8 *Str);

void UART_receiveString(uint8 *Str); // Receive until #


#endif /* UART_DRIVER_UART_H_ */
