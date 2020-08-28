/*
 * password.c
 *
 *  Created on: Jun 17, 2020
 *      Author: shahira ibrahim
 */

#include "password.h"
#include "EEPROM.h"
#include "keypad.h"
#include "lcd.h"

volatile uint8 g_guestPass[GUEST_PASSWORD_SIZE];
volatile uint8 g_adminPass[ADMIN_PASSWORD_SIZE];
volatile uint8 data=0;

bool PASS_existOrNot(uint8 size,uint16 addr){
	uint8  i,count=0;
	uint8 data=0;
	for(i=0;i<size;i++){
		data=EEPROM_readByte(addr+i);
		if(data == 255) count++;
	}
	if(count == size)
		return FALSE;
	else
		return TRUE;

}


void PASS_insert(uint8 size,uint8 * password){
	sint8 i;
	for(i=0;i<size;i++){
		password[i]=KeyPad_getPressedKey();
		LCD_goToRowColumn(1,11+i);
		if(password[i] >= 0 && password[i] <=9)
		{
			LCD_intgerToString(password[i]);
			_delay_ms(300);
			LCD_goToRowColumn(1,11+i);
			LCD_displayCharacter('*');
		}




	}

}

void PASS_saving(uint8 size,uint8 * password,uint16 addr){
	uint8 i;
	for(i=0;i<size;i++){
		EEPROM_writeByte(password[i], addr+i);

	}
}

bool PASS_compare(uint8 * password,uint8 size,uint16 addr){
   uint8 i,data=0;
   for(i=0;i<size;i++){
	   data=EEPROM_readByte(addr+i);
	   _delay_ms(5);
	   if(data != password[i])
	   {
		   return FALSE;
	   }
   }
   return TRUE;
}
