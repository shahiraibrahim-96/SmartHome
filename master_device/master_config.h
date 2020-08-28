/*
 * master_config.h
 *
 *  Created on: Jun 17, 2020
 *      Author: shahira ibrahim
 */

#ifndef MASTER_CONFIG_H_
#define MASTER_CONFIG_H_
/****************************************modes pins*****************************************************/
#define ADMIN_PIN  PA4
#define GUEST_PIN  PA5
#define BLOCK_PIN  PA6

#define ADMIN_DATA_OUT PORTA
#define GUEST_DATA_OUT PORTA
#define BLOCK_DATA_OUT PORTA

#define ADMIN_DIR_PORT  DDRA
#define GUEST_DIR_PORT  DDRA
#define BLOCK_DIR_PORT  DDRA

#define ADMIN_MODE  0
#define GUEST_MODE  1

#define ATTEMPTS_NUMBER 3


/***********************************************************************************************/

/***************************************TIME OUT CONFIGURATION*********************************/

#define SESSION_TIME_OUT 90
#define BLOCK_TIME   20
#define STILL_IN_BLOCK_TIME 1
#define OUT_OF_BLOCK_MODE   0
#define BLOCK_FLAG_ADDRESS  20
#define ON  1
#define OFF 2
#define ROOM1_ADMIN_MODE 1
#define ROOM2_ADMIN_MODE 2
#define ROOM3_ADMIN_MODE 3
#define MORE_ADMIN_MENU  4
#define ROOM4_ADMIN_MENU 1
#define RET_TO_ADMIN_MENU 4
#define RET_SELECT   0
#define TV    2
#define AIR_COND 3
#define AIR_COND_SET_TEMP 1
#define AIR_COND_CNTRL    2
#define AIR_COND_RET      0
#define ROOM1_GUEST_MODE  1
#define ROOM2_GUEST_MODE  2
#define ROOM3_GUEST_MODE  3
#define ROOM4_GUEST_MODE  4
#define RET_TO_GUEST_MENU 0
/******************************************commands************************************************************/

#define MESSAGE_RECEIVED_ACK   0xFF

#define SEND_STATUS_ROOM1     0x11
#define SEND_STATUS_ROOM2     0x22
#define SEND_STATUS_ROOM3     0x33
#define SEND_STATUS_ROOM4     0x44
#define SEND_STATUS_TV        0x55
#define SEND_STATUS_AC        0x66

#define TURN_ROOM1_ON   0x01
#define TURN_ROOM2_ON   0x02
#define TURN_ROOM3_ON   0x03
#define TURN_ROOM4_ON   0x04
#define TURN_TV_ON      0x05
#define TURN_AC_ON      0x06
#define SET_AC_TEMPERATURE  0x07
#define CHECK_AC_CONDITION       0x08

#define TURN_ROOM1_OFF   0x09
#define TURN_ROOM2_OFF   0x10
#define TURN_ROOM3_OFF   0x20
#define TURN_ROOM4_OFF   0x30
#define TURN_TV_OFF      0x40
#define TURN_AC_OFF      0x50

#define ON_STATUS      0x01
#define OFF_STATUS     0x00

#endif /* MASTER_CONFIG_H_ */
