/*
 * motor.c
 *
 *
 *      Author: Mahmoud Ibrahim
 *      august 2022
 *      updated December 2022
 */
#include "motor.h"


void MOTOR_RotateClockWise(void){

	MOTOR_PORT&=(~(1<<MOTOR_PIN_1 ));
	MOTOR_PORT|=(1<<MOTOR_PIN_2 );
}
void MOTOR_RotateAntiClockWise(void){

	MOTOR_PORT|=(1<<MOTOR_PIN_1 );
	MOTOR_PORT&=(~(1<<MOTOR_PIN_2 ));
}
void MOTOR_Stop(void){

	MOTOR_PORT&=(~(1<<MOTOR_PIN_1 ));
	MOTOR_PORT&=(~(1<<MOTOR_PIN_2 ));
}
