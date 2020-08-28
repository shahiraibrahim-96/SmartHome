/*
 * EEPROM.h
 *
 *  Created on: Jun 18, 2020
 *      Author: shahira ibrahim
 */

#ifndef EEPROM_H_
#define EEPROM_H_

#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"

void EEPROM_writeByte(uint8 byte,uint16 address);
uint8 EEPROM_readByte(uint16 address);


#endif /* EEPROM_H_ */
