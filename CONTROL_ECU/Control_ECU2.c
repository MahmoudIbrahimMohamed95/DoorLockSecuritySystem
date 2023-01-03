/*
 * Control_ECU.c
 *
 *      Author: Mahmoud Ibrahim
 *      august 2022
 *      updated December 2022
 */


#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"

#include "G_Timer.h"
#include "uart.h"

#include "EEPROM.h"
#include "motor.h"
#include "Buzzer.h"

#define POSITIVE  0
#define NEGATIVE  1

#define NOT_MATCH        (0u)	/*in both micro-controllers use this flag for non match password*/
#define MATCH            (1u)	/*in both micro-controllers use this flag for match password*/
#define OPEN_DOOR        (2u)	/*in both micro-controllers flag to open door */
#define BUZZER_ON        (3u)	/*in both micro-controllers flag to trigger Buzzer */
#define ENTER_PASSWORD   (4u)	/*in both micro-controllers use this flag as Enter password Flag*/
#define REPEAT_PASSWORD  (5u)  /*in both micro-controllers use this flag as REPEAT password Flag*/
#define GET_OPTION       (6u)  /*in both micro-controllers use this flag to Getting Options*/
#define READY			 (7u)	/*in both micro-controllers - Ready flag*/
#define DONE		   	 (8u)	/*in both micro-controllers - Done flag*/

static uint8 Saving_PasswordFlag=0;
static uint8 Checking_PasswordFlag=0;
static uint8 Matching_Fault=0;
static uint8 recieved_buff1[5];
static uint8 recieved_buff2[5];
static uint8 recieved_buff3[5];
static uint8 system_options;

volatile static uint16 Timer0_Ovf_Count=0; 		/*counting no. of overflows for timer0*/

volatile static uint8 Timer1_Ovf_Count=0;		/*counting no. of overflows for timer1*/

//volatile static uint16 Timer2_Ovf_Count=0;		/*counting no. of overflows for timer2*/

volatile static uint8 Busywait_Flag=0 ;		    /*busy-wait flag*/

UART_Config	 UART_Config_Type = {Normal_Speed , disabled , one_bit  ,  eight_bits };

TIMER_Config Timer0_Config_Type = {TIMER0 , F_CPU_1024 , NORMALMODE , 0 , 0};  /*for starting timer count*/

TIMER_Config Timer1_Config_Type = {TIMER1 , F_CPU_1024 , NORMALMODE , 0 , 0};  /*for starting timer count*/

TIMER_Config Timer2_Config_Type = {TIMER2 , F_CPU_1024 , NORMALMODE , 0 , 0};  /*for starting timer count*/

TIMER_Config Timer0_Config_Type2= {TIMER0 , NO_CLOCK , NORMALMODE , 0 , 0};        /*for stopping timer count*/

TIMER_Config Timer1_Config_Type2= {TIMER1 , NO_CLOCK , NORMALMODE , 0 , 0};	       /*for stopping timer count*/

TIMER_Config Timer2_Config_Type2= {TIMER2 , NO_CLOCK , NORMALMODE , 0 , 0};    /*for starting timer count*/


 void APP2_TIMER0_Processing(void);
 void APP_TIMER1_Processing(void);
 void APP1_TIMER0_Processing(void);

 void Get_SavePassword(void);
 void Get_CheckPassword(void);

 void Open_Door(void);
 uint8 Get_option(void);

int main(void){

	SET_BIT (DDRC , 0 );	/*I2c SCLA PIN as output*/
	SET_BIT( DDRC , 1);		/*I2c SDA PIN as output*/
	CLEAR_BIT( DDRD , 0);	/*uart Rx PIN as input*/
	SET_BIT( DDRD , 1);		/*uart Tx PIN as output*/
	SET_BIT( DDRD , 2); 	/*Motor pin*/
	SET_BIT( DDRD , 3);  	/*Motor pin*/
	SET_BIT( DDRD , 4);		/*Buzzer pin*/
	SET_BIT( SREG , 7);		/*Enable global interrupt*/

	UART_init(&UART_Config_Type);
	EEPROM_init();

	while(UART_recieveByte() != READY){;}

	Get_SavePassword();
	Get_CheckPassword();

	while(1){

		system_options = Get_option();

		switch(system_options){

		case POSITIVE:
			Get_SavePassword();break;

		case NEGATIVE:
			Open_Door();break;

		default:	break;

		}
	}
		return 0;
}


void Get_SavePassword(void){

uint8	EEPROM_returnStatus;

	Saving_PasswordFlag=0;

	while(Saving_PasswordFlag==0){

		UART_sendByte(ENTER_PASSWORD );

		for(uint8 i=0;i<5;i++){
			recieved_buff1[i]= UART_recieveByte();
		}

		UART_sendByte(REPEAT_PASSWORD);

		for(uint8 i=0;i<5;i++){
			recieved_buff2[i]= UART_recieveByte();

		}

		for(uint8 i=0;i<5;i++){

			if(recieved_buff2[i] != recieved_buff1[i]){
				UART_sendByte(NOT_MATCH);
				break;
			}

			else{
				if(i==4){
					for(uint8 j=0 ; j<5; j++){

						EEPROM_returnStatus = EEPROM_writeByte((0x0000 + (0x0001 * j)) , 5);
						_delay_ms(20);
					}
					for(uint8 j=0 ; j<5; j++){

						EEPROM_returnStatus = EEPROM_writeByte((0x0000 + (0x0001 * j)) , recieved_buff2[j]);
						_delay_ms(20);
					}

					UART_sendByte(MATCH);
					Saving_PasswordFlag=1;
				}
			}
		}
	}
}

void Get_CheckPassword(void){

	uint8 local_holder=0;
	uint8 EEPROM_returnstatus=0;

	Checking_PasswordFlag=0;

	while(Checking_PasswordFlag==0){

		if(Matching_Fault== 3){

			Matching_Fault=0;

			Timer1_Set_CallBack(APP_TIMER1_Processing);
			Timer_Init(&Timer1_Config_Type);

			UART_sendByte(BUZZER_ON);
			Buzzer_On();

			while(Busywait_Flag==0){;}

			Timer_stop(&Timer1_Config_Type2);
			Busywait_Flag=0;

			Buzzer_Off();
			UART_sendByte(DONE);
		}

		UART_sendByte(ENTER_PASSWORD);

		for(uint8 i=0;i<5;i++){
			recieved_buff3[i]= UART_recieveByte();
		}

		for(uint8 i=0;i<5;i++){
			local_holder=0;

			EEPROM_returnstatus= EEPROM_readByte((0x0000+(i*0x0001)) , &local_holder);
			_delay_ms(20);


			if(  local_holder != recieved_buff3[i]){
				UART_sendByte(NOT_MATCH);
				Matching_Fault++;
				break;
			}
			else{
				if(i==4){
					UART_sendByte(MATCH);
					Checking_PasswordFlag=1;
					Matching_Fault=0;
				}
			}
		}
	}
}

uint8 Get_option(void){

	uint8 local_Option=0;

	UART_sendByte(GET_OPTION);

	local_Option= UART_recieveByte();

	return local_Option;
}

void Open_Door(void){

	UART_sendByte(OPEN_DOOR);

	Timer0_Set_CallBack(APP1_TIMER0_Processing);
	Timer_Init(&Timer0_Config_Type);


	MOTOR_RotateClockWise(); 		//opening door and waiting 15sec

	while(Busywait_Flag==0){;}
	Timer_stop(&Timer0_Config_Type2);
	Busywait_Flag=0;

	Timer0_Set_CallBack(APP2_TIMER0_Processing);
	Timer_Init(&Timer0_Config_Type);


	UART_sendByte(DONE);

	MOTOR_Stop();      				//keep door open for 3sec

	while(Busywait_Flag==0){;}
	Timer_stop(&Timer0_Config_Type2);
	Busywait_Flag=0;



	Timer0_Set_CallBack( APP1_TIMER0_Processing);
	Timer_Init(&Timer0_Config_Type);


	UART_sendByte(DONE);

	MOTOR_RotateAntiClockWise();	//closing door and waiting 15sec

	while(Busywait_Flag==0){;}
	Timer_stop(&Timer0_Config_Type2);
	Busywait_Flag=0;


	MOTOR_Stop();      				//stop motor


}


void APP1_TIMER0_Processing(void){

	Timer0_Ovf_Count++;

	if(Timer0_Ovf_Count== 480){
		Timer0_Ovf_Count=0;
		Busywait_Flag=1;
	}
}

void APP2_TIMER0_Processing(void){

	Timer0_Ovf_Count++;

	if(Timer0_Ovf_Count== 92){
		Timer0_Ovf_Count=0;
		Busywait_Flag=1;
	}
}

void APP_TIMER1_Processing(void){

	Timer1_Ovf_Count++;

	if(Timer1_Ovf_Count== 8){

		Timer1_Ovf_Count=0;
		Busywait_Flag=1;
	}
}
