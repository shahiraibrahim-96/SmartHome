/*
 * EEPROM.c
 *
 *  Created on: Jun 18, 2020
 *      Author: shahira ibrahim
 */

#include "EEPROM.h"

void EEPROM_writeByte(uint8 byte,uint16 address){


	while(EECR &(1<<EEWE));
	EEAR = address;
	EEDR = byte;
	EECR |=(1<<EEMWE);
	EECR =(1<<EEWE);


}


uint8 EEPROM_readByte(uint16 address){

	while(EECR &(1<<EEWE));
	EEAR = address;
	EECR =(1<<EERE);
	return EEDR ;

}
