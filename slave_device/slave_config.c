/*
 * slave_config.c
 *
 *  Created on: Jun 27, 2020
 *      Author: shahira ibrahim
 */

#include "slave_config.h"
#include "adc.h"

void init_modules(void){
	SET_BIT(ROOM1_DIR_PORT,ROOM1_PIN);
	SET_BIT(ROOM2_DIR_PORT,ROOM2_PIN);
	SET_BIT(ROOM3_DIR_PORT,ROOM3_PIN);
	SET_BIT(ROOM4_DIR_PORT,ROOM4_PIN);
	SET_BIT(AC_DIR_PORT,AC_PIN );
	SET_BIT(TV_DIR_PORT,TV_PIN);

	CLEAR_BIT(ROOM1_DATA_OUT,ROOM1_PIN);
	CLEAR_BIT(ROOM2_DATA_OUT,ROOM2_PIN);
	CLEAR_BIT(ROOM3_DATA_OUT,ROOM3_PIN);
	CLEAR_BIT(ROOM4_DATA_OUT,ROOM4_PIN);
	CLEAR_BIT(AC_DATA_OUT,AC_PIN);
	CLEAR_BIT(TV_DATA_OUT,TV_PIN);

	 ADC_init();
}
