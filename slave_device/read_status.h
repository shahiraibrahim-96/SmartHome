/*
 * read_status.h
 *
 *  Created on: Jun 27, 2020
 *      Author: shahira ibrahim
 */

#ifndef READ_STATUS_H_
#define READ_STATUS_H_

#include "std_types.h"
#include "micro_config.h"
#include "common_macros.h"



uint8 ROOM_readStatus(uint8 roomNum);
bool TV_readStatus(void);
bool AC_readStatus(void);
uint16 TEMP_returnValue(void);

#endif /* READ_STATUS_H_ */
