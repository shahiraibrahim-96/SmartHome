/*
 * slave_config.h
 *
 *  Created on: Jun 27, 2020
 *      Author: shahira ibrahim
 */

#ifndef SLAVE_CONFIG_H_
#define SLAVE_CONFIG_H_

#include "std_types.h"
#include "micro_config.h"
#include "common_macros.h"

#define ROOM1_PIN    PD0
#define ROOM2_PIN    PD1
#define ROOM3_PIN    PD2
#define ROOM4_PIN    PD3
#define AC_PIN  PD7
#define TV_PIN  PD5
#define TEMP_SENSOR_CHANNEL  (0u)


#define ROOM1_DIR_PORT     DDRD
#define ROOM2_DIR_PORT     DDRD
#define ROOM3_DIR_PORT     DDRD
#define ROOM4_DIR_PORT     DDRD

#define AC_DIR_PORT      DDRD
#define TV_DIR_PORT      DDRD
#define TEMP_SENSOR_DIR_PORT   DDRA

#define ROOM1_READ_DATA   PIND
#define ROOM2_READ_DATA   PIND
#define ROOM3_READ_DATA   PIND
#define ROOM4_READ_DATA   PIND

#define AC_READ_DATA      PIND
#define TV_READ_DATA      PIND

#define ROOM1_DATA_OUT  PORTD
#define ROOM2_DATA_OUT  PORTD
#define ROOM3_DATA_OUT  PORTD
#define ROOM4_DATA_OUT  PORTD

#define AC_DATA_OUT  PORTD
#define TV_DATA_OUT  PORTD
#define TEMP_SENSOR_DATA_OUT PORTA





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
#endif /* SLAVE_CONFIG_H_ */


void init_modules(void);
