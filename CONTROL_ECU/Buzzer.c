/*
 * Buzzer.c
 *
 *  Created on: Dec 25, 2022
 *      Author: K I N G
 */
#include "Buzzer.h"

void Buzzer_On(void){


	BUZZER_PORT|=(1<<BUZZER_PIN);
}

void Buzzer_Off(void){

	BUZZER_PORT&=(~(1<<BUZZER_PIN));
}
