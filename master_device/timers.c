/*
 * timers.c
 *
 *  Created on: May 13, 2019
 *      Author: shahira ibrahim
 */
#include "timers.h"
#include "lcd.h"
#include "master_config.h"

volatile uint8 g_timeOut=0;
volatile uint8 count_sessionTime=0;
volatile uint8 count_interrupts=0;
volatile bool g_timeIsUp=FALSE;
volatile uint8 	INTERRUPT_FLAG=0;
/*********************************************FUNCTIONS DEFINITION***********************************/

ISR(TIMER1_COMPA_vect){
	LCD_goToRowColumn(1,5);
	LCD_displayString("  ");
	g_timeOut++;
	LCD_goToRowColumn(1,5);
	LCD_intgerToString(BLOCK_TIME-g_timeOut);

 }

ISR(TIMER0_OVF_vect){
	count_interrupts++;
  if(count_interrupts == NUMBER_OF_INTERRUPTS_ONE_SECOND){
	  count_interrupts=0;
	  count_sessionTime++;
  }
  if(count_sessionTime >= SESSION_TIME_OUT){
	  g_timeIsUp=TRUE;
	  INTERRUPT_FLAG=1;
  }

}

/*function description :function for initiating timer 0 in normal mode*/
 void TIMER0_normalMode(TIMERS_Prescaler prescaler){

	/*start count From zero*/
	TCNT0=0;

	/*enable over flow interrupt*/
	TIMSK =(1<<TOIE0);

	 /*enable globle interrupt*/
	 SREG  |= (1<<7);

	/*setting the required frequency*/
	TCCR0 =(1<<FOC0) | prescaler ;
}


 /*function description :function for initiating timer 0 in CTC mode*/
 void TIMER0_CTCMode(TIMERS_Prescaler prescaler,uint8 compareValue){

	/*start from zero*/
	TCNT0=0;

	/*setting compare value*/
	OCR0=compareValue;

	/*enable interrupt for compare match output mode*/
	TIMSK =(1<<OCIE0);

	 /*enable globle interrupt*/
	 SREG  |= (1<<7);

	/*setting timer 0  in ctc mode by setting WGM01,FOC0 bits and setting the required frequency*/
	TCCR0 =(1<<FOC0)|(1<<WGM01) | prescaler;


}

 /*function description :function for initiating timer 0 in PWM mode*/
 void TIMER0_PWMMode(TIMERS_Prescaler prescaler,uint8 dutyCycle){

	/*start from zero*/
	TCNT0=0;

	/*setting the required duty cycle*/
	OCR0=dutyCycle;

	/*setting OCR0/PB3 as an output pin*/
	DDRB |=(1<<PB3);

	/*setting fast PWM
	 * NON INVERTED MODE
	 * the required frequency
	 */
	TCCR0 =(1<<WGM01)|(1<<WGM00)|(1<<COM01) | prescaler;

}



 /*function description :function for initiating timer 1 in normal mode*/
 void TIMER1_normalMode(TIMERS_Prescaler prescaler){

	/*start count from zero*/
	TCNT1=34286;

	/*enable over flow interrupt*/
	TIMSK =(1<<TOIE1);

	/*setting those two bits in non PWM mode*/
	TCCR1A =(1<<FOC1B) |(1<<FOC1A);

	/*setting the required frequency*/
	TCCR1B = prescaler;
}

/*function description :function for initiating timer 1 in CTC mode*/
 void TIMER1_CTCMode(TIMERS_Prescaler prescaler,uint16 compareValue){

	 /*start count from zero*/
	 TCNT1=0;

	 /*setting compare value*/
	 OCR1A=compareValue;

	 /*enable interrupt for compare match output mode*/
	 TIMSK |=(1<<OCIE1A);

	 /*setting those two bits in non PWM mode*/
	 TCCR1A =(1<<FOC1B) |(1<<FOC1A);

	 /*setting timer 1  in ctc mode by setting WGM12 bit and setting the required frequency*/
	 TCCR1B =(1<<WGM12) | (1<<CS12);

	 /*enable globle interrupt*/
	 SREG  |= (1<<7);
}

 /*function description :function for initiating timer 1 in PWM mode*/
 void TIMER1_PWMMode(TIMERS_Prescaler prescaler,uint16 dutyCycle){

	 /*start count from zero*/
	 TCNT1=0;

	 /*setting the required duty cycle*/
	 OCR1A=dutyCycle;

	 /*setting OCR1A/PD3 as an output pin*/
	 DDRD =(1<<PD5);

	 /*setting fast PWM
	  * NON INVERTED MODE
	  *the required frequency
	  */
	 TCCR1A =(1<<WGM10)|(1<<WGM11) |(1<<COM1A1);
	 TCCR1B =(1<<WGM12) | (1<<WGM13)| prescaler;

}



 /*function description :function for initiating timer 2 in normal mode*/
 void TIMER2_normalMode(TIMERS_Prescaler prescaler){

	 /*start count from zero*/
	 TCNT2=0;

	/*enable over flow interrupt*/
	TIMSK =(1<<TOIE2);

	/*setting the required frequency*/
	TCCR2 =(1<<FOC2) | prescaler ;

}

 /*function description :function for initiating timer 2 in CTC mode*/
 void TIMER2_CTCMode(TIMERS_Prescaler prescaler,uint8 compareValue){

	/*start count from zero*/
	TCNT2=0;

	/*setting compare value*/
	OCR2=compareValue;

	 /*enable interrupt for compare match output mode*/
	TIMSK =(1<<OCIE2);

	/*setting timer 2 in ctc mode by setting WGM21,FOC2 bits*/
	TCCR2 =(1<<FOC2)|(1<<WGM21);

	/*setting the required frequency*/
	TCCR2 |= prescaler;

}

 /*function description :function for initiating timer 2 in PWM mode*/
 void TIMER2_PWMMode(TIMERS_Prescaler prescaler,uint8 dutyCycle){

	/*start count from zero*/
	TCNT2=0;

	/*setting the required duty cycle*/
	OCR2=dutyCycle;

	/*setting OCR2/PD7 as an output pin*/
	DDRD |=(1<<PD7);

	/*setting fast PWM
	 * NON INVERTED MODE
	 *the required frequency
	 */
	TCCR2 =(1<<WGM20)|(1<<WGM21)|(1<<COM21)| prescaler;

}

void TIMER0_OFF(void){
	TCCR0 =0;
	TCNT0=0;
	TIMSK=0;
	SREG =0;
}

 void TIMER1_OFF(void){
	 TCCR1A=0;
	 TCCR1B=0;
	 TCNT1=0;
	 TIMSK=0;
	 SREG =0;
 }
void TIMER2_OFF(void){
	TCCR2 =0;
	TCNT2=0;
	TIMSK=0;
	SREG =0;
}

