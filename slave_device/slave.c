/*
 * SLAVE.c
 *
 *  Created on: Jun 17, 2020
 *      Author: shahira ibrahim
 */

#include "timers.h"
#include "spi.h"
#include "slave_config.h"
#include "read_status.h"
#include "lcd.h"

volatile uint16 g_tempRequired=100;
volatile bool AC_pervious_state=OFF_STATUS;
volatile uint16 g_tempMeasured=0;
volatile bool g_AC_TURN_ON=FALSE;

ISR(TIMER1_COMPA_vect){
	 g_tempMeasured=TEMP_returnValue();
	 if(g_AC_TURN_ON){
		if(g_tempMeasured < g_tempRequired  )
		{
			CLEAR_BIT(AC_DATA_OUT, AC_PIN );
			AC_pervious_state=OFF_STATUS;

		}
		else if(g_tempMeasured > g_tempRequired)
		{
		     SET_BIT(AC_DATA_OUT, AC_PIN );
		     AC_pervious_state=ON_STATUS;
		}
		else
		{
			if(AC_pervious_state==ON_STATUS)
				SET_BIT(AC_DATA_OUT, AC_PIN );

			else
				CLEAR_BIT(AC_DATA_OUT, AC_PIN );

		}
	 }

}

int main(){

	uint8 command=0xFF;
	SPI_initSlave();
	init_modules();
	LCD_init();
	TIMER1_CTCMode(FCPU_256,3125);

	while(1){


		command = SPI_tranceiver(0XFF);




		switch(command){

		case SEND_STATUS_ROOM1 :


			 SPI_tranceiver(ROOM_readStatus(ROOM1_PIN));
			break;


		case SEND_STATUS_ROOM2 :
			 SPI_tranceiver(ROOM_readStatus(ROOM2_PIN));
			break;

		case SEND_STATUS_ROOM3 :
			 SPI_tranceiver(ROOM_readStatus(ROOM3_PIN));
			break;

		case SEND_STATUS_ROOM4 :
			 SPI_tranceiver(ROOM_readStatus(ROOM4_PIN));
			break;

		case SEND_STATUS_TV :
			 SPI_tranceiver(TV_readStatus());
			break;

		case SEND_STATUS_AC:
			SPI_tranceiver(AC_readStatus());
			break;

		case TURN_ROOM1_ON:
			SET_BIT(ROOM1_DATA_OUT,ROOM1_PIN );
			break;

		case TURN_ROOM2_ON:
			SET_BIT(ROOM2_DATA_OUT,ROOM2_PIN );
			break;

		case TURN_ROOM3_ON:
			SET_BIT(ROOM3_DATA_OUT,ROOM3_PIN );
			break;

		case TURN_ROOM4_ON:
			SET_BIT(ROOM4_DATA_OUT,ROOM4_PIN );
			break;

		case TURN_TV_ON:
			SET_BIT( TV_DATA_OUT, TV_PIN );
			break;

		case TURN_AC_ON :
			TIMER1_CTCMode(FCPU_256,3125);
			g_AC_TURN_ON=TRUE;
			break;

		case TURN_ROOM1_OFF:
			CLEAR_BIT(ROOM1_DATA_OUT,ROOM1_PIN);
			break;

		case TURN_ROOM2_OFF:
			CLEAR_BIT(ROOM2_DATA_OUT,ROOM2_PIN);
			break;

		case TURN_ROOM3_OFF:
			CLEAR_BIT(ROOM3_DATA_OUT,ROOM3_PIN);
			break;

		case TURN_ROOM4_OFF:
			CLEAR_BIT(ROOM4_DATA_OUT,ROOM4_PIN);
			break;

		case TURN_TV_OFF:
			CLEAR_BIT(TV_DATA_OUT, TV_PIN);
			break;

		case TURN_AC_OFF:
			CLEAR_BIT(AC_DATA_OUT, AC_PIN );
			g_AC_TURN_ON=FALSE;
			AC_pervious_state=OFF_STATUS;
			TIMER1_OFF();
			break;

		case SET_AC_TEMPERATURE:
			g_tempRequired = SPI_recieveByte();
			break;



		}


	}
}
