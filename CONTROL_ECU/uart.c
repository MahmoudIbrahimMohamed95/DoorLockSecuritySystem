/*
 * uart.c
 *
 *  Created on: Aug 26, 2021
 *      Author: Mahmoud Ibrahim Mohamed
 */

#include "uart.h"

uint8 BAUD_PRESCALE;


void UART_init(UART_Config* Config_ptr)

{
	UCSRA=(UCSRA & 0xFD) | (((Config_ptr->TRANSMISSION_SPEED)&(0x01))<<1); //U2X value

	/************************** UCSRB Description **************************
	 * RXCIE = 0 Disable USART RX Complete Interrupt Enable
	 * TXCIE = 0 Disable USART Tx Complete Interrupt Enable
	 * UDRIE = 0 Disable USART Data Register Empty Interrupt Enable
	 * RXEN  = 1 Receiver Enable
	 * RXEN  = 1 Transmitter Enable
	 * UCSZ2 = 0 For 8-bit data mode
	 * RXB8 & TXB8 not used for 8-bit data mode
	 ***********************************************************************/
	UCSRB = (1<<RXEN) | (1<<TXEN);

	UCSRB = ((UCSRB & 0xFB) | (((Config_ptr->DATA_BITS)  & 0x04 )<<2 )); //higher bit in data bit number selection

	UCSRC|= (1<<URSEL); //accessing UCSRC

	UCSRC= ((UCSRC & 0xCF) | (((Config_ptr->PARITY)  & 0x03 )<<4 ));

	UCSRC= ((UCSRC & 0xF8) | (((Config_ptr->STOP_BIT) & 0x01)<<3 ));

	UCSRC= ((UCSRC & 0xF9) | (((Config_ptr->DATA_BITS) & 0x03)<<1));// lower 2 bits in data bit number selection

	/* First 8 bits from the BAUD_PRESCALE inside UBRRL and last 4 bits in UBRRH*/

	if( Config_ptr->TRANSMISSION_SPEED == Normal_Speed)
	{
		BAUD_PRESCALE = ((F_CPU / ((Config_ptr->BAUD_RATE) * 16UL)) - 1) ;  //Normal speed transmit
	}

	else if( Config_ptr->TRANSMISSION_SPEED == Double_Speed)
	{
		 BAUD_PRESCALE = ((F_CPU / ((Config_ptr->BAUD_RATE) * 8UL)) - 1); //double speed transmit
	}
	else{
		;
	}

	UBRRH = BAUD_PRESCALE>>8;
	UBRRL = BAUD_PRESCALE;
}

void UART_sendByte(const uint8 data)
{

	while(BIT_IS_CLEAR(UCSRA,UDRE)){}
	UDR = data;
	/************************* Another Method *************************
	UDR = data;
	while(BIT_IS_CLEAR(UCSRA,TXC)){} // Wait until the transimission is complete TXC = 1
	SET_BIT(UCSRA,TXC); // Clear the TXC flag
	*******************************************************************/
}

uint8 UART_recieveByte(void)
{
	while(BIT_IS_CLEAR(UCSRA,RXC)){}
    return UDR;
}

void UART_sendString(const uint8 *Str)
{
	uint8 i = 0;
	while(Str[i] != '\0')
	{
		UART_sendByte(Str[i]);
		i++;
	}
	/************************* Another Method *************************
	while(*Str != '\0')
	{
		UART_sendByte(*Str);
		Str++;
	}
	*******************************************************************/
}

void UART_receiveString(uint8 *Str)
{
	uint8 i = 0;
	Str[i] = UART_recieveByte();
	while(Str[i] != '#')
	{
		i++;
		Str[i] = UART_recieveByte();
	}
	Str[i] = '\0';
}
