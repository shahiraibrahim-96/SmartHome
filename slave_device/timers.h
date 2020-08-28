/*
 * timers.h
 *
 *  Created on: May 13, 2019
 *      Author: shahira ibrahim
 */

#ifndef TIMERS_H_
#define TIMERS_H_

#include "std_types.h"
#include "micro_config.h"
#include "common_macros.h"



/***************************************data type declaration*****************************************/

/*data type declaration for timers prescaler*/
typedef enum{
	N0_CLOCK,FCPU_CLOCK,FCPU_8,FCPU_64,FCPU_256,FCPU_1024
}TIMERS_Prescaler;






/*******************************************function prototype*************************************/

/*function description :function for initiating timer 0 in normal mode*/
void TIMER0_normalMode(TIMERS_Prescaler prescaler);

/*function description :function for initiating timer 0 in CTC mode*/
void TIMER0_CTCMode(TIMERS_Prescaler prescaler,uint8 compareValue);

/*function description :function for initiating timer 0 in PWM mode*/
void TIMER0_PWMMode(TIMERS_Prescaler prescaler,uint8 dutyCycle);

/*function description :function for initiating timer 1 in normal mode*/
void TIMER1_normalMode(TIMERS_Prescaler prescaler);

/*function description :function for initiating timer 1 in CTC mode*/
void TIMER1_CTCMode(TIMERS_Prescaler prescaler,uint16 compareValue);

/*function description :function for initiating timer 1 in PWM mode*/
void TIMER1_PWMMode(TIMERS_Prescaler prescaler,uint16 dutyCycle);

/*function description :function for initiating timer 2 in normal mode*/
void TIMER2_normalMode(TIMERS_Prescaler prescaler);

/*function description :function for initiating timer 2 in CTC mode*/
void TIMER2_CTCMode(TIMERS_Prescaler prescaler,uint8 compareValue);

/*function description :function for initiating timer 2 in PWM mode*/
void TIMER2_PWMMode(TIMERS_Prescaler prescaler,uint8 dutyCycle);

/*function for disable timer0 module*/
void TIMER0_OFF(void);

/*function for disable timer1 module*/
void TIMER1_OFF(void);

/*function for disable timer2 module*/
void TIMER2_OFF(void);

#endif /* TIMERS_H_ */
