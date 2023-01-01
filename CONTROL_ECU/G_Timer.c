/*
 * G_Timer.c
 *
 *  Created on: Aug 25, 2022
 *      Author: K I N G
 */


#include "G_Timer.h"

/*****************************************************************************************************************
                                        Global Variables Definations
****************************************************************************************************************/
static volatile void (*g_Timer0_CallBackPtr)(void)= NULL_PTR;
static volatile void (*g_Timer1_CallBackPtr)(void)= NULL_PTR;
static volatile void (*g_Timer2_CallBackPtr)(void)= NULL_PTR;


/****************************************************************************************************************
 *                                      Interrupt Service Routine
 ***************************************************************************************************************/
ISR(TIMER0_OVF_vect){

	if(g_Timer0_CallBackPtr != NULL_PTR)
		{
			/* Call the Call Back function in the application after the edge is detected */
			(*g_Timer0_CallBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
		}
}
ISR(TIMER0_COMP_vect){

	if(g_Timer0_CallBackPtr != NULL_PTR)
		{
			/* Call the Call Back function in the application after the edge is detected */
			(*g_Timer0_CallBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
		}
}

ISR(TIMER1_OVF_vect){

	if(g_Timer1_CallBackPtr != NULL_PTR)
		{
			/* Call the Call Back function in the application after the edge is detected */
			(*g_Timer1_CallBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
		}
}
ISR(TIMER1_COMPA_vect){

	if(g_Timer1_CallBackPtr != NULL_PTR)
		{
			/* Call the Call Back function in the application after the edge is detected */
			(*g_Timer1_CallBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
		}
}

ISR(TIMER2_OVF_vect){

	if(g_Timer2_CallBackPtr != NULL_PTR)
		{
			/* Call the Call Back function in the application after the edge is detected */
			(*g_Timer2_CallBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
		}
}
ISR(TIMER2_COMP_vect){

	if(g_Timer2_CallBackPtr != NULL_PTR)
		{
			/* Call the Call Back function in the application after the edge is detected */
			(*g_Timer2_CallBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
		}
}

/****************************************************************************************************************
 *                                    functions Definitions
 ***************************************************************************************************************/
void Timer_Init(const TIMER_Config*Config_Ptr){

	switch (Config_Ptr-> timer){

	case TIMER0:

		switch( Config_Ptr-> mode){

		case NORMALMODE :

			TCNT0 = Config_Ptr->intial_value; //Set Timer initial value
			TIMSK |= (1<<TOIE0); // Enable Timer0 Overflow Interrupt
			TCCR0=0x80; //config control reg
			TCCR0 = ((TCCR0)&(0xF8)) | ((Config_Ptr-> clock ) & (0x07));

			break;

		case COMPAREMODE :

			TCNT0 = Config_Ptr->intial_value;    // Set Timer initial value

			OCR0  = Config_Ptr->compare_value; // Set Compare Value

			TIMSK |= (1<<OCIE0); // Enable Timer0 Compare Interrupt
			/* Configure timer0 control register
			 * 1. Non PWM mode FOC0=1
			 * 2. CTC Mode WGM01=1 & WGM00=0
			 * 3. No need for OC0 in this example so COM00=0 & COM01=0
			 */
			TCCR0 = 0x88;

			TCCR0 = ((TCCR0) & (0xF8)) | ((Config_Ptr-> clock ) & (0x07));

			break;

		default:

			break;
		}
		break;

	case TIMER1:

		switch( Config_Ptr-> mode){

		case NORMALMODE :

			//Set Timer initial value

			TCNT1L = Config_Ptr->intial_value;
			TCNT1H=  ((Config_Ptr->intial_value) >>8);

			TIMSK |= (1<<TOIE1); // Enable Timer1 Overflow Interrupt
			TCCR1A=0x0C;
			TCCR1B=0x00;
			TCCR1B = ((TCCR1B)&(0xF8)) | ((Config_Ptr-> clock ) & (0x07));

			break;

		case COMPAREMODE :

			TCNT1L =    Config_Ptr->intial_value;

			TCNT1H=   ((Config_Ptr->intial_value) >>8);

			TIMSK |=  (1<<TOIE1); // Enable Timer1 Overflow Interrupt

			OCR1AL  =   Config_Ptr->compare_value; // Set Compare Value

			OCR1AH  = ( Config_Ptr->compare_value >> 8); // Set Compare Value

			TCCR1A=0x0C;

			TCCR1B=0x08;

			TCCR1B = ((TCCR1B)&(0xF8)) | ((Config_Ptr-> clock ) & (0x07));

			break;


		default:

			break;
		}

		break;

	case TIMER2:

		switch( Config_Ptr-> mode){

		case NORMALMODE :

			TCNT2 = Config_Ptr->intial_value; //Set Timer initial value
			TIMSK |= (1<<TOIE2); // Enable Timer0 Overflow Interrupt
			TCCR2=0x80;
			TCCR2 = ((TCCR2)&(0xF8)) | ((Config_Ptr-> clock ) & (0x07));

			break;

		case COMPAREMODE :

			TCNT2 = Config_Ptr->intial_value;    // Set Timer initial value

			OCR2  = Config_Ptr->compare_value; // Set Compare Value

			TIMSK |= (1<<OCIE2); // Enable Timer0 Compare Interrupt
			/* Configure timer0 control register
			 * 1. Non PWM mode FOC0=1
			 * 2. CTC Mode WGM01=1 & WGM00=0
			 * 3. No need for OC0 in this example so COM00=0 & COM01=0
			 */
			TCCR2 = 0x88;

			TCCR2 = ((TCCR2) & (0xF8)) | ((Config_Ptr-> clock ) & (0x07));

			break;

		default:

			break;
		}

		break;

	}
}

void Timer0_Set_CallBack(void(*a_ptr0)(void)){

	g_Timer0_CallBackPtr = a_ptr0;
}

void Timer1_Set_CallBack(void(*a_ptr1)(void)){

	g_Timer1_CallBackPtr = a_ptr1;
}

void Timer2_Set_CallBack(void(*a_ptr2)(void)){

	g_Timer2_CallBackPtr = a_ptr2;
}

void Timer_stop (const TIMER_Config*Config_Ptr){

	switch(Config_Ptr-> timer){

	case TIMER0:

		switch( Config_Ptr-> mode){

		case NORMALMODE:

			TCNT0 = 0x00; //Set Timer initial value to 0
			TIMSK &=~(1<<TOIE0); // disEnable Timer0 Overflow Interrupt
			TCCR0=0x00;
			break;

		case COMPAREMODE:

			TCNT0 = 0x00;    // Set Timer initial value to 0
			OCR0  = 0x00; // Set Compare Value
			TIMSK &=~(1<<OCIE0); // disEnable Timer0 Compare Interrupt
			TCCR0 = 0x00;
			break;

		default:
			break;
		}

		break;

	case TIMER1:

		switch( Config_Ptr-> mode){

		case NORMALMODE :

			//Set Timer initial value

			TCNT1L = 0x00 ;
			TCNT1H=  0x00;

			TIMSK &=~(1<<TOIE1); // disable Timer1 Overflow Interrupt

			TCCR1A= 0x00;

			TCCR1B=  0x00;

			break;

		case COMPAREMODE :

			TCNT1L =  0x00;

			TCNT1H=   0x00;

			TIMSK &=~ (1<<TOIE1); // disable Timer1 Overflow Interrupt

			OCR1AL  =   0x00; // Set Compare Value

			OCR1AH  = 0x00; // Set Compare Value

			TCCR1A=0x00;

			TCCR1B=0x00;

			break;


    	default:

	    	break;
	}

	break;

	case TIMER2:

		switch (Config_Ptr-> mode){

		case NORMALMODE:

			TCNT2 = 0x00; //Set Timer initial value to 0
			TIMSK &=~(1<<TOIE2); // Disable Timer2 Overflow Interrupt
			TCCR2=0x00;

			break;
		case COMPAREMODE:

			TCNT2 = 0x00;    // Set Timer initial value to 0
			OCR2  = 0x00; // Set Compare Value 0
			TIMSK &=~(1<<OCIE2); // disable Timer2 Compare Interrupt

			TCCR2 = 0x00;
			break;

		default:
			break;
		}

		break;

	default:

			break;
	}

}
