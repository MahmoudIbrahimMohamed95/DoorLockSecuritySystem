/*
 * Buzzer.h
 *
 *      Author: Mahmoud Ibrahim
 *      august 2022
 *      updated December 2022
 */

#ifndef BUZZER_H_
#define BUZZER_H_

#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"

#define BUZZER_PIN    4
#define BUZZER_PORT   PORTD


 void Buzzer_On(void);
 void Buzzer_Off(void);

#endif /* BUZZER_H_ */
