/*
 * Buzzer.c
 *
 *
 *      Author: Mahmoud Ibrahim
 *      august 2022
 *      updated December 2022
 */
#include "Buzzer.h"

void Buzzer_On(void){


	BUZZER_PORT|=(1<<BUZZER_PIN);
}

void Buzzer_Off(void){

	BUZZER_PORT&=(~(1<<BUZZER_PIN));
}
