/*
 * password.h
 *
 *  Created on: Jun 17, 2020
 *      Author: shahira ibrahim
 */

#ifndef PASSWORD_H_
#define PASSWORD_H_


#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"

#define ADMIN_PASSWORD_SIZE 5
#define GUEST_PASSWORD_SIZE 5
#define GUEST_ADDRESS  5
#define ADMIN_ADDRESS  0

extern volatile uint8 g_guestPass[GUEST_PASSWORD_SIZE];
extern volatile uint8 g_adminPass[ADMIN_PASSWORD_SIZE];

/********************************************function declaration*******************************/
bool PASS_existOrNot(uint8 size,uint16 addr);
void PASS_insert(uint8 size,uint8 * password);
void PASS_saving(uint8 size,uint8 * password,uint16 addr);
bool PASS_compare(uint8 * password,uint8 size,uint16 addr);
#endif /* PASSWORD_H_ */
