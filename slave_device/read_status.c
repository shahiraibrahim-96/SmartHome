/*
 * read_status.c
 *
 *  Created on: Jun 27, 2020
 *      Author: shahira ibrahim
 */


#include "read_status.h"
#include "slave_config.h"
#include "adc.h"
#include "spi.h"
#include "lcd.h"



uint8 ROOM_readStatus(uint8 roomNum){


	uint8 g_roomStatus=OFF_STATUS;
	switch(roomNum){

	case ROOM1_PIN :
		if( BIT_IS_SET(ROOM1_READ_DATA ,ROOM1_PIN))
			g_roomStatus=ON_STATUS;
		else
			g_roomStatus=OFF_STATUS;
		break;

	case ROOM2_PIN :
		if( BIT_IS_SET(ROOM2_READ_DATA ,ROOM2_PIN))
			g_roomStatus=ON_STATUS;
		else
			g_roomStatus=OFF_STATUS;
		break;

	case ROOM3_PIN :
		if( BIT_IS_SET(ROOM3_READ_DATA ,ROOM3_PIN))
			g_roomStatus=ON_STATUS;
		else
			g_roomStatus=OFF_STATUS;
		break;
	case ROOM4_PIN :
		if( BIT_IS_SET(ROOM4_READ_DATA ,ROOM4_PIN))
			g_roomStatus=ON_STATUS;
		else
			g_roomStatus=OFF_STATUS;
		break;


	}
 return g_roomStatus;

}


bool TV_readStatus(void){

	bool g_TV_status=FALSE;

	if(BIT_IS_SET(TV_READ_DATA ,TV_PIN))
		g_TV_status=ON_STATUS;
	else
		g_TV_status=OFF_STATUS;

	   return g_TV_status;
}

bool AC_readStatus(void){

	bool g_AC_status=FALSE;
    if(BIT_IS_SET(AC_READ_DATA ,AC_PIN))
	    g_AC_status= ON_STATUS;
    else
	    g_AC_status=OFF_STATUS;
	   return g_AC_status;
}

uint16 TEMP_returnValue(void){
	uint16 temp=0;
	temp=ADC_readChannel(TEMP_SENSOR_CHANNEL);
	temp=(temp*150*5)/(1023*1.5);
	return temp;
}
