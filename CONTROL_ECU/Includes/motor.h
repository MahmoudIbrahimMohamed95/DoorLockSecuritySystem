/*
 * motor.h
 *
 *
 *      Author: Mahmoud Ibrahim
 *      august 2022
 *      updated December 2022
 */
#ifndef MOTOR_H_
#define MOTOR_H_


#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"

#define MOTOR_PIN_1  2
#define MOTOR_PIN_2	 3
#define MOTOR_PORT   PORTD

void MOTOR_RotateClockWise(void);
void MOTOR_RotateAntiClockWise(void);
void MOTOR_Stop(void);
#endif /* MOTOR_H_ */
