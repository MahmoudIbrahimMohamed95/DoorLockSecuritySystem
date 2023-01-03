/*
 * G_Timer.h
 *
 *
 *      Author: Mahmoud Ibrahim
 *      august 2022
 *      updated December 2022
 */
#ifndef GENERAL_TIMER_DRIVER_G_TIMER_H_
#define GENERAL_TIMER_DRIVER_G_TIMER_H_


#include "common_macros.h"
#include "micro_config.h"
#include "std_types.h"


/******************************************************************************************************************
 *											types declaration                                                    *
******************************************************************************************************************/

typedef enum
{

	TIMER0 , TIMER1 , TIMER2

}TIMER;

typedef enum
{

	NO_CLOCK , F_CPU_CLOCK , F_CPU_8 , F_CPU_16 , F_CPU_64 , F_CPU_256 , F_CPU_1024

}TIMER_Clock;

typedef enum
{

	NORMALMODE , COMPAREMODE

}TIMER_Mode;

typedef struct{

	TIMER timer;

	TIMER_Clock clock;

	TIMER_Mode  mode;

	uint8 intial_value;

	uint8 compare_value;

}TIMER_Config;

/******************************************************************************************************************
										function prototype
******************************************************************************************************************/

void Timer_Init(const TIMER_Config * Config_Ptr);

void Timer0_Set_CallBack(void(*a_ptr0)(void));

void Timer1_Set_CallBack(void(*a_ptr1)(void));

void Timer2_Set_CallBack(void(*a_ptr2)(void));

void Timer_stop (const TIMER_Config*Config_Ptr);


#endif /* GENERAL_TIMER_DRIVER_G_TIMER_H_ */
