/*
 * hmi_ecu2.c
 *
 *  Created on: Dec 26, 2022
 *      Author: K I N G
 */



#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"

#include "G_Timer.h"
#include "uart.h"

#include "keypad.h"
#include "lcd.h"


/****************************
 *
 ****************************/

#define NOT_MATCH        (0u) 		/*in both micro-controllers use this theme for non match password*/

#define MATCH            (1u)		/*in both micro-controllers use this theme for match password*/

#define OPEN_DOOR        (2u)		/*in both micro-controllers flag to open door */

#define BUZZER_ON        (3u)		/*in both micro-controllers flag to trigger Buzzer */

#define ENTER_PASSWORD   (4u)		/*in both micro-controllers use this flag as Enter password Flag*/

#define REPEAT_PASSWORD  (5u)  	/*in both micro-controllers use this flag as REPEAT password Flag*/

#define GET_OPTION       (6u)  	/*in both micro-controllers use this flag to Getting Options*/

#define READY			 (7u)		/*in both micro-controllers - Ready flag*/

#define DONE			 (8u)		/*in both micro-controllers - Done flag*/

#define ENTER           (13u)  	/*Enter pressing-Key to send password*/


/****************************
 *
 ****************************/

static uint8 key;		  						/*a variable carry pressed key value*/

static uint8 password[5]; 	 					/*an array to getting password from user*/


volatile static uint16 Timer0_Ovf_Count=0; 		/*counting no. of overflows for timer0*/

volatile static uint8 Timer1_Ovf_Count=0;		/*counting no. of overflows for timer1*/

//volatile static uint16 Timer2_Ovf_Count=0;		/*counting no. of overflows for timer2*/

volatile static uint8 Busywait_Flag=0 ;			/*busy-wait flag*/



/************************
 *
 ***********************/
UART_Config	 UART_Config_Type = {Normal_Speed , disabled , one_bit  ,  eight_bits };

TIMER_Config Timer0_Config_Type = {TIMER0 , F_CPU_1024 , NORMALMODE , 0 , 0};  /*for starting timer count*/

TIMER_Config Timer1_Config_Type = {TIMER1 , F_CPU_1024 , NORMALMODE , 0 , 0};  /*for starting timer count*/

TIMER_Config Timer2_Config_Type = {TIMER2 , F_CPU_1024 , NORMALMODE , 0 , 0};  /*for starting timer count*/

TIMER_Config Timer0_Config_Type2= {TIMER0 , NO_CLOCK , NORMALMODE , 0 , 0};    /*for stopping timer count*/

TIMER_Config Timer1_Config_Type2= {TIMER1 , NO_CLOCK , NORMALMODE , 0 , 0};	   /*for stopping timer count*/

TIMER_Config Timer2_Config_Type2= {TIMER2 , NO_CLOCK , NORMALMODE , 0 , 0};    /*for starting timer count*/



/*****************************
 *
 ****************************/

void GetSendPassWordSerially(void);

void GetPassword( uint8 * ptr );

void SendPasswordSerially( const uint8 * ptr  );

void APP2_TIMER0_Processing(void);

void APP_TIMER1_Processing(void);

void APP1_TIMER0_Processing(void);


/****************************
 *
 ***************************/

int main(void){


	uint8 local_RecievedOrder;

	CLEAR_BIT( DDRD , 0);	/*uart Rx PIN as output*/
	SET_BIT( DDRD , 1);		/*uart Tx PIN as output*/
	SET_BIT(SREG,7);		/*Enable global interrupt*/

	UART_init(&UART_Config_Type);
	LCD_init();

	UART_sendByte(READY) ;

	while(1){

		local_RecievedOrder= UART_recieveByte();

		switch(local_RecievedOrder){

		case NOT_MATCH:

			LCD_sendCommand(CLEAR_COMMAND);
			LCD_displayStringRowColumn(0,0,"not_matched");

			break;

		case MATCH:

			LCD_sendCommand(CLEAR_COMMAND);
			LCD_displayStringRowColumn(0,0,"matching");

			break;

		case OPEN_DOOR:

/*
			Timer0_Set_CallBack(APP1_TIMER0_Processing);
			Timer_Init(&Timer0_Config_Type);
*/
			LCD_sendCommand(CLEAR_COMMAND);
			LCD_displayStringRowColumn(0,0,"DOOR IS OPEN");

/*
	 		while(Busywait_Flag==0){;}
			Timer_stop(&Timer0_Config_Type2);
			Busywait_Flag=0;
*/
			while( UART_recieveByte() != DONE);

/*
	 		Timer0_Set_CallBack(APP2_TIMER0_Processing);
			Timer_Init(&Timer0_Config_Type);
*/
			LCD_sendCommand(CLEAR_COMMAND);
			LCD_displayStringRowColumn(0,0,"Stop_Moving");
/*
			while(Busywait_Flag==0){;}
			Timer_stop(&Timer0_Config_Type2);
			Busywait_Flag=0;
*/
			while( UART_recieveByte() != DONE);

/*
	 		Timer0_Set_CallBack(APP1_TIMER0_Processing);
			Timer_Init(&Timer0_Config_Type);
*/
			LCD_sendCommand(CLEAR_COMMAND);
			LCD_displayStringRowColumn(0,0,"DOOR IS LOCKED");

/*
	  		while(Busywait_Flag==0){;}
			Timer_stop(&Timer0_Config_Type2);
			Busywait_Flag=0;
*/
			break;

		case BUZZER_ON:

			LCD_sendCommand(CLEAR_COMMAND);
			LCD_displayStringRowColumn(0,0,"ERROR");

/*	1min delay buzzer on */

			Timer1_Set_CallBack(APP_TIMER1_Processing);
			Timer_Init(&Timer1_Config_Type);

			while(Busywait_Flag==0);
			Timer_stop(&Timer1_Config_Type2);
			Busywait_Flag=0;
			LCD_sendCommand(CLEAR_COMMAND);

			break;


		case ENTER_PASSWORD:

				LCD_sendCommand(CLEAR_COMMAND);
				LCD_displayStringRowColumn(0,0,"Enter_password");
				GetSendPassWordSerially();

				break;

		case REPEAT_PASSWORD:

				LCD_sendCommand(CLEAR_COMMAND);
				LCD_displayStringRowColumn(0,0,"Repeat_Password");
				GetSendPassWordSerially();

				break;


		case GET_OPTION:

				LCD_sendCommand(CLEAR_COMMAND);
				LCD_displayStringRowColumn(0,0,"ChangePass  0");
				LCD_displayStringRowColumn(1,0,"OpenDoor    1");

				key=KeyPad_getPressedKey();    /*Getting Option*/
				UART_sendByte(key) ;

				break;

		default:
				break;
			}

		}

		return 0;
}

void GetSendPassWordSerially(void){

	GetPassword(password);

	SendPasswordSerially( password);

}

void GetPassword( uint8 * ptr  ){

	for(uint8 i=0 ; i<5 ;i++){

		key = KeyPad_getPressedKey();

		if( (key <= 9) && (key >=0) ){
			ptr[i]=key;
			LCD_displayStringRowColumn(1,i,"*");
    	}

		else{
			i=i-1;
		}

		_delay_ms(500);			/*pressing time*/
	}

}

void SendPasswordSerially( const uint8 * ptr  ){

	while(KeyPad_getPressedKey() != ENTER ){;}

	for(uint8 i=0 ; i <5 ;i++){

		UART_sendByte(ptr[i]);

	}

}

void APP2_TIMER0_Processing(void){

	Timer0_Ovf_Count++;

	if(Timer0_Ovf_Count== 92){
		Timer0_Ovf_Count=0;
		Busywait_Flag=1;
	}
}

void APP1_TIMER0_Processing(void){

	Timer0_Ovf_Count++;

	if(Timer0_Ovf_Count== 480 ){
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
