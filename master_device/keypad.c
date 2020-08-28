 /******************************************************************************
 *
 * Module: KEYPAD
 *
 * File Name: keypad.c
 *
 * Description: Source file for the Keypad driver
 *
 * Author: shahira ibrahim
 *
 *******************************************************************************/

#include "keypad.h"
#include "timers.h"
volatile uint8 key=NOT_PRESSED;
/*******************************************************************************
 *                      Functions Prototypes(Private)                          *
 *******************************************************************************/

#if (N_col == 3)
/*
 * Function responsible for mapping the switch number in the keypad to
 * its corresponding functional number in the proteus for 4x3 keypad
 */
static uint8 KeyPad_4x3_adjustKeyNumber(uint8 button_number);
#elif (N_col == 4)
/*
 * Function responsible for mapping the switch number in the keypad to
 * its corresponding functional number in the proteus for 4x4 keypad
 */
static uint8 KeyPad_4x4_adjustKeyNumber(uint8 button_number);
#endif

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
uint8 KeyPad_getPressedKey(void)
{
	uint8 col,row;
	while(1 )
	{

		if(INTERRUPT_FLAG){
			INTERRUPT_FLAG=0;
			break;
		}
		else{

			for(col=0;col<N_col;col++) /* loop for columns */
			{
				/*
				 * each time only one of the column pins will be output and
				 * the rest will be input pins include the row pins
				 */
				KEYPAD_PORT_DIR = (0b00010000<<col);

				/*
				 * clear the output pin column in this trace and enable the internal
				 * pull up resistors for the rows pins
				 */
				KEYPAD_PORT_OUT = (~(0b00010000<<col));

				for(row=0;row<N_row;row++) /* loop for rows */
				{
					if(BIT_IS_CLEAR(KEYPAD_PORT_IN,row)) /* if the switch is press in this row */
					{

						#if (N_col == 3)
							return KeyPad_4x3_adjustKeyNumber((row*N_col)+col+1);
						#elif (N_col == 4)
							return KeyPad_4x4_adjustKeyNumber((row*N_col)+col+1);
						#endif

					}
				}
		    }
		}


	}
}
#if (N_col == 3) 

static uint8 KeyPad_4x3_adjustKeyNumber(uint8 button_number)
{
	switch(button_number)
	{
		case 10:
			     key=PRESSED;
		     	 return '*'; // ASCII Code of *
				 break;
		case 11:
			     key=PRESSED;
			     return 0;
				 break;		
		case 12:
			    key=PRESSED;
			    return '#'; // ASCII Code of #
				break;
		default:
			  key=PRESSED;
			return button_number;
	}
} 

#elif (N_col == 4)
 
static uint8 KeyPad_4x4_adjustKeyNumber(uint8 button_number)
{
	switch(button_number)
	{
		case 1:
			   key=PRESSED;
			   return 7;
				break;
		case 2:
			    key=PRESSED;
			    return 8;
				break;
		case 3:key=PRESSED;
			return 9;

				break;
		case 4:key=PRESSED;
			return '%'; // ASCII Code of %

				break;
		case 5:key=PRESSED;
			return 4;

				break;
		case 6:key=PRESSED;
			return 5;

				break;
		case 7:key=PRESSED;
			return 6;

				break;
		case 8:key=PRESSED;
			return '*'; /* ASCII Code of '*' */

				break;		
		case 9:key=PRESSED;
			return 1;

				break;
		case 10:key=PRESSED;
			return 2;
				break;
		case 11:key=PRESSED;
			return 3;

				break;
		case 12:key=PRESSED;
			return '-'; /* ASCII Code of '-' */

				break;
		case 13:key=PRESSED;
			return 13;  /* ASCII of Enter */

				break;			
		case 14:key=PRESSED;
			return 0;

				break;
		case 15:key=PRESSED;
			return '='; /* ASCII Code of '=' */

				break;
		case 16:key=PRESSED;
			return '+'; /* ASCII Code of '+' */

				break;
		default:
			key=PRESSED;
			return button_number;
	}
} 

#endif
