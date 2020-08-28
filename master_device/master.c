/*
 * application.c
 *
 *  Created on: Jun 17, 2020
 *      Author: shahira ibrahim
 */
#include "keypad.h"
#include "lcd.h"
#include "spi.h"
#include "master_config.h"
#include "password.h"
#include "timers.h"
#include "EEPROM.h"

volatile bool g_adminPassSaved=FALSE;
volatile bool g_guestPassSaved=FALSE;

volatile uint8 password[5]={0};
volatile uint8 temperature=0;


int main(void){

	sint8 module=-1,option=-1,more_modules=-1,airCond_select=-1,mode=-1;
	uint8 count_attempts=0;
	bool password_status=FALSE;
	uint8 g_room1Status= OFF_STATUS;
	uint8 g_room2Status= OFF_STATUS;
	uint8 g_room3Status= OFF_STATUS;
	uint8 g_room4Status= OFF_STATUS;
	uint8 g_TV_status= OFF_STATUS;
	uint8 g_AC_status= OFF_STATUS;

	SPI_initMaster(); //initialization first micro as master device
	LCD_init();      // LCD initialization routine


	SET_BIT(ADMIN_DIR_PORT, ADMIN_PIN); //set ADMIN pin as a output pin
	SET_BIT(GUEST_DIR_PORT,GUEST_PIN);  //set GUEST pin as an output pin
	SET_BIT(BLOCK_DIR_PORT,BLOCK_PIN);  //set BLOCK pin as an output pin
	CLEAR_BIT(ADMIN_DATA_OUT,ADMIN_PIN); //clear ADMIN pin
	CLEAR_BIT(GUEST_DATA_OUT,GUEST_PIN); //clear GUEST pin
	CLEAR_BIT(BLOCK_DATA_OUT,BLOCK_PIN); //clear BLOCK pin

	//check if it's the first time to use the device by checking if there are admin and guest passwords
	g_adminPassSaved=PASS_existOrNot(ADMIN_PASSWORD_SIZE,ADMIN_ADDRESS);
	g_guestPassSaved=PASS_existOrNot(GUEST_PASSWORD_SIZE, GUEST_ADDRESS);

	/*display "welcome smart Home App" on LCD*/
	LCD_displayString("*******welcome*******");
	LCD_displayStringRowColumn(1,0,"Smart Home App");

	/*halt the system so the user can see 'welcome' sentence*/
	_delay_ms(650);

	/*clear LCD screen*/
	LCD_clearScreen();

	/*check if this the first time the system is being activated by the user*/
	if((!g_adminPassSaved) && (!g_guestPassSaved) ){

		/*display "login for first" sentence on LCD */
		LCD_displayString("login for first");

		/*display "time" sentence on LCD */
		LCD_displayStringRowColumn(1,0,"time");

		/*halt the system so the user can see the sentences in the first and the second row*/
		_delay_ms(800);

		/*clear LCD screen*/
		LCD_clearScreen();

		/*display "set admin pass" sentence on the first row  of the LCD*/
		LCD_displayStringRowColumn(0,0,"set admin pass");

		/*display "Admin pass:" sentence on LCD */
		LCD_displayStringRowColumn(1,0,"Admin pass:");

		/*enter admin password for first time*/
		PASS_insert(ADMIN_PASSWORD_SIZE,g_adminPass);

		/*clear LCD screen*/
		LCD_clearScreen();

		/*display "set guest pass" sentence on LCD */
		LCD_displayStringRowColumn(0,0,"set guest pass");

		/*display "Guest pass:" sentence on LCD */
		LCD_displayStringRowColumn(1,0,"Guest pass:");

		/*enter guest password for first time*/
		PASS_insert(GUEST_PASSWORD_SIZE,g_guestPass);

		/*save admin password in EEPROM memory*/
		PASS_saving(ADMIN_PASSWORD_SIZE,g_adminPass,ADMIN_ADDRESS);

		/*save guest password in EEPROM memory*/
		PASS_saving(GUEST_PASSWORD_SIZE,g_guestPass,GUEST_ADDRESS);

		/* clear Block_mode flag */
		EEPROM_writeByte(OUT_OF_BLOCK_MODE,BLOCK_FLAG_ADDRESS);

	}


	while(1){

		/*first check that the system is on block mode*/
		if(EEPROM_readByte(BLOCK_FLAG_ADDRESS)==STILL_IN_BLOCK_TIME){

			/* start timer1 on CTC mode*/
			TIMER1_CTCMode(FCPU_256,31250);

			/*clear LCD screen*/
			LCD_clearScreen();

			/*display "Login Blocked" sentence on the first row of the LCD */
			LCD_displayStringRowColumn(0,0,"Login Blocked");

			/*display "wait 20 seconds" sentence on the second row of the LCD */
			LCD_displayStringRowColumn(1,0,"wait 20 seconds");

			/*turn Block pin ON */
			SET_BIT(BLOCK_DATA_OUT,BLOCK_PIN);

			/*wait until the timer counts 20 seconds*/
			while(g_timeOut < BLOCK_TIME);

			/*turn off the timer*/
			TIMER1_OFF();

			/* clear the timer_out flag*/
			g_timeOut=0;

			/*turn block pin off*/
			CLEAR_BIT(BLOCK_DATA_OUT,BLOCK_PIN);

			/* clear Block_mode flag */
			EEPROM_writeByte(OUT_OF_BLOCK_MODE,BLOCK_FLAG_ADDRESS);

			/*clear LCD screen*/
			LCD_clearScreen();
		}

		/* check if session time is up*/
		if(count_sessionTime >= SESSION_TIME_OUT){

			/*turn off timer zero*/
			TIMER0_OFF();

			/*clear count_sessionTime variable*/
			count_sessionTime=0;

			/*clear count_interrupts variable*/
			count_interrupts=0;

			/*clear LCD screen*/
			LCD_clearScreen();

			/*display "session TimeOut" sentence on the first row of the LCD */
			LCD_displayStringRowColumn(0,0,"session TimeOut");

			/* turn off admin led*/
			CLEAR_BIT(ADMIN_DATA_OUT,ADMIN_PIN);

			/* turn off guest led*/
			CLEAR_BIT(GUEST_DATA_OUT,GUEST_PIN);

			/*halt the system for 500 ms*/
			_delay_ms(500);

		}

		/*check if the system was previously entered block mode or not*/
		if(EEPROM_readByte(BLOCK_FLAG_ADDRESS) == OUT_OF_BLOCK_MODE){

			do{
				/*clear LCD screen*/
				LCD_clearScreen();

				do{

					/*display "select mode" sentence on the first row of the LCD */
					LCD_displayStringRowColumn(0,0,"select mode");

					/*display "0:admin 1:Guest" sentence on the second row of the LCD */
					LCD_displayStringRowColumn(1,0,"0:admin 1:Guest");

					/* the user should enter the required mode*/
					mode = KeyPad_getPressedKey();

					/*halt the system for 500 ms*/
					_delay_ms(500);

					/*check if the user entered an invalid input*/
					if(mode != ADMIN_MODE && mode != GUEST_MODE && mode !=-1)
					{
						/*clear LCD screen*/
						LCD_clearScreen();

						/*display "invalid mode" sentence on the first row of the LCD */
						LCD_displayString("invalid mode");

						/*halt the system for 500 ms*/
						_delay_ms(500);
					}

					/*keep looping until the user chooses any valid mode*/
				}while((mode!= ADMIN_MODE && mode!= GUEST_MODE ));



				do{

					/*clear LCD screen*/
					LCD_clearScreen();

					/*check if it's Admin mode or guest mode*/
					if(mode == ADMIN_MODE)

						/*display "Admin Mode" sentence on the first row of the LCD */
						LCD_displayStringRowColumn(0,0,"Admin Mode");
					else

						/*display "Guest Mode" sentence on the first row of the LCD */
						LCD_displayStringRowColumn(0,0,"Guest Mode");

					/*display "Enter Pass:" sentence on the second row of the LCD */
					LCD_displayStringRowColumn(1,0,"Enter Pass:");

					/*user enter the password*/
					PASS_insert(ADMIN_PASSWORD_SIZE,password);

					/*check if it's admin mode or guest mode*/
					if(mode == ADMIN_MODE)

						/*compare the password that user entered with one inside EEPROM memory*/
						password_status=PASS_compare(password,ADMIN_PASSWORD_SIZE,ADMIN_ADDRESS);
					else

						/*compare the password that user entered with one inside EEPROM memory*/
						password_status=PASS_compare(password,GUEST_PASSWORD_SIZE,GUEST_ADDRESS);

                   /* if the password that the user entered is wrong execute this block of instructions*/
					if(!password_status) {

						/*clear LCD screen*/
						LCD_clearScreen();

						/*increase count_attempts flag by one*/
						count_attempts++;

						/*display "Wrong password" sentence on the first row of the LCD */
						LCD_displayStringRowColumn(0,0,"Wrong password");

						/*display "tries left:" sentence on the second row of the LCD */
						LCD_displayStringRowColumn(1,0,"tries left:");


						/*make the cursor move to the second row column 11*/
						LCD_goToRowColumn(1,11);

						LCD_intgerToString(ATTEMPTS_NUMBER - count_attempts);


						/*halt the system for 500 ms*/
						_delay_ms(500);


					}

					else
					{

						/*clear LCD screen*/
						LCD_clearScreen();

						/*display "Right password" sentence on the first row of the LCD */
						LCD_displayStringRowColumn(0,0,"Right password");

						/* in case of Admin mode*/
						if(mode == ADMIN_MODE){

							/*display "Admin Mode" sentence on the second row of the LCD */
							LCD_displayStringRowColumn(1,0,"Admin Mode");

							/*turn on Admin mode led*/
							SET_BIT(ADMIN_DATA_OUT,ADMIN_PIN);
						}

						/*in case of guest mode*/
						else{

							/*display "Guest Mode" sentence on the second row of the LCD */
							LCD_displayStringRowColumn(1,0,"Guest Mode");

							/*turn on guest mode led*/
							SET_BIT(GUEST_DATA_OUT,GUEST_PIN);
						}

						/*halt the system for 800 ms*/
						_delay_ms(800);

						/* get out of the loop*/
						break;
					}

			    /* keep looping until he writes the right password or it reaches to the maximum number of attempts*/
				}while((count_attempts < ATTEMPTS_NUMBER)  && (!password_status) );

				/*if the user reaches the max num of attempts to enter the right password*/
				if(count_attempts == ATTEMPTS_NUMBER ){

					/*start timer 1 in CTC mode using 256 prescaler*/
					TIMER1_CTCMode(FCPU_256,31250);

					/* set the block mode flag*/
					EEPROM_writeByte(STILL_IN_BLOCK_TIME,BLOCK_FLAG_ADDRESS);

					/*clear LCD screen*/
					LCD_clearScreen();

					/*display "Login Blocked" sentence on the first row of the LCD */
					LCD_displayStringRowColumn(0,0,"Login Blocked");

					/*display "wait 20 seconds" sentence on the second row of the LCD */
					LCD_displayStringRowColumn(1,0,"wait 20 seconds");

					/*turn on block mode led*/
					SET_BIT(BLOCK_DATA_OUT,BLOCK_PIN);

					/* wait until the timer complete its 20 seconds for block mode*/
					while(g_timeOut < BLOCK_TIME);

					/*turn off timer 1*/
					TIMER1_OFF();

					/*clear g_timeOut flag*/
					g_timeOut=0;

					/*turn off block mode led*/
					CLEAR_BIT(BLOCK_DATA_OUT,BLOCK_PIN);

					/* clear block mode flag*/
					EEPROM_writeByte(OUT_OF_BLOCK_MODE,BLOCK_FLAG_ADDRESS);

					/*clear LCD screen*/
					LCD_clearScreen();

				}

				/*clear count_attempts flag*/
				count_attempts=0;

			 /*keep looping until the user write the right password*/
			}while(!password_status);

		}



		if(EEPROM_readByte(BLOCK_FLAG_ADDRESS) == OUT_OF_BLOCK_MODE ){
			g_timeIsUp=FALSE;
			TIMER0_normalMode(FCPU_1024);
			while(!g_timeIsUp){
				LCD_clearScreen();
				more_modules=-1,option=-1,module=-1,option=-1,airCond_select=-1;
				LCD_displayStringRowColumn(0,0,"1:Room1 2:Room2");
				if(mode == ADMIN_MODE){
					LCD_displayStringRowColumn(1,0,"3:Room3 4:More");
					_delay_ms(500);
					module = KeyPad_getPressedKey();

					switch(module){

					case  ROOM1_ADMIN_MODE:

						while(option !=RET_SELECT && !g_timeIsUp){
							SPI_tranceiver(SEND_STATUS_ROOM1);
							_delay_ms(100);
							g_room1Status=SPI_tranceiver(0xFF);
							LCD_clearScreen();
							LCD_displayStringRowColumn(0,0,"Room1  S:");
							if(g_room1Status==ON_STATUS )
								LCD_displayStringRowColumn(0,9,"ON");
							else if(g_room1Status==OFF_STATUS )
								LCD_displayStringRowColumn(0,9,"OFF");
							LCD_displayStringRowColumn(1,0,"1:ON 2:OFF 0:RET");
							_delay_ms(250);
							option = KeyPad_getPressedKey();
							if(option == ON ){
								SPI_sendByte(TURN_ROOM1_ON );
								break;
							}
							else if(option == OFF){
								SPI_sendByte(TURN_ROOM1_OFF );
								break;
							}
							else if(option !=RET_SELECT){
								LCD_clearScreen();
								LCD_displayStringRowColumn(0,0,"wrong input");
								_delay_ms(500);
							}
						}

						break;


					case  ROOM2_ADMIN_MODE:
						while(option !=RET_SELECT && !g_timeIsUp){
							SPI_tranceiver(SEND_STATUS_ROOM2);
							_delay_ms(100);
							g_room2Status=SPI_tranceiver(0xFF);
							LCD_clearScreen();
							LCD_displayStringRowColumn(0,0,"Room2  S:");
							if( g_room2Status==ON_STATUS )
								LCD_displayStringRowColumn(0,8,"ON");
							else
								LCD_displayStringRowColumn(0,8,"OFF");
							LCD_displayStringRowColumn(1,0,"1:ON 2:OFF 0:RET");
							_delay_ms(250);
							option = KeyPad_getPressedKey();
							if(option == ON ){
								SPI_sendByte(TURN_ROOM2_ON );
								break;
							}
							else if(option == OFF){
								SPI_sendByte(TURN_ROOM2_OFF );
								break;
							}
							else if(option !=RET_SELECT){
								LCD_clearScreen();
								LCD_displayStringRowColumn(0,0,"wrong input");
								_delay_ms(500);
							}
						}

						break;



					case  ROOM3_ADMIN_MODE:
						while(option !=RET_SELECT && !g_timeIsUp){
							SPI_tranceiver(SEND_STATUS_ROOM3);
							_delay_ms(100);
							g_room3Status=SPI_tranceiver(0xFF);
							LCD_clearScreen();
							LCD_displayStringRowColumn(0,0,"Room3  S:");
							if(g_room3Status==ON_STATUS )
								LCD_displayStringRowColumn(0,8,"ON");
							else
								LCD_displayStringRowColumn(0,8,"OFF");
							LCD_displayStringRowColumn(1,0,"1:ON 2:OFF 0:RET");
							_delay_ms(250);
							option = KeyPad_getPressedKey();
							if(option == ON ){
								SPI_sendByte(TURN_ROOM3_ON );
								break;
							}
							else if(option == OFF){
								SPI_sendByte(TURN_ROOM3_OFF );
								break;
							}
							else if(option !=RET_SELECT){
								LCD_clearScreen();
								LCD_displayStringRowColumn(0,0,"wrong input");
								_delay_ms(500);
							}
						}

						break;


					case MORE_ADMIN_MENU:

						while(more_modules != RET_TO_ADMIN_MENU && !g_timeIsUp){
							airCond_select=-1,more_modules=-1,option=-1;
							LCD_clearScreen();
							LCD_displayStringRowColumn(0,0,"1:Room4 2:TV");
							LCD_displayStringRowColumn(1,0,"3:AirCond 4:RET");
							_delay_ms(250);
							more_modules=KeyPad_getPressedKey();
							switch(more_modules){
							case ROOM4_ADMIN_MENU:
								while(option !=RET_SELECT && !g_timeIsUp ){
									SPI_tranceiver(SEND_STATUS_ROOM4);
									_delay_ms(100);
									g_room4Status=SPI_tranceiver(0xFF);
									LCD_clearScreen();
									LCD_displayStringRowColumn(0,0,"Room4  S:");
									LCD_displayStringRowColumn(1,0,"1:ON 2:OFF 0:RET");
									if(g_room4Status==ON_STATUS )
										LCD_displayStringRowColumn(0,9,"ON");
									else if(g_room4Status==OFF_STATUS)
										LCD_displayStringRowColumn(0,9,"OFF");
									_delay_ms(250);
									option = KeyPad_getPressedKey();
									if(option == ON ){
										SPI_sendByte(TURN_ROOM4_ON );
										break;
									}
									else if(option == OFF){
										SPI_sendByte(TURN_ROOM4_OFF );
										break;
									}
									else if(option !=RET_SELECT && !g_timeIsUp){
										LCD_clearScreen();
										LCD_displayStringRowColumn(0,0,"wrong input");
										_delay_ms(500);
									}

								}

								break;

							case TV:
								while(option !=RET_SELECT && !g_timeIsUp){
									LCD_clearScreen();
									LCD_displayStringRowColumn(0,0,"TV   S:");
									LCD_displayStringRowColumn(1,0,"1:ON 2:OFF 0:RET");
									SPI_tranceiver(SEND_STATUS_TV);
									_delay_ms(100);
									g_TV_status=SPI_tranceiver(0xFF);
									if(g_TV_status==ON_STATUS )
										LCD_displayStringRowColumn(0,7,"ON");
									else
										LCD_displayStringRowColumn(0,7,"OFF");
									_delay_ms(250);
									option = KeyPad_getPressedKey();
									if(option == ON ){
										SPI_sendByte(TURN_TV_ON );

										break;
									}
									else if(option == OFF){
										SPI_sendByte(TURN_TV_OFF );
										break;
									}
									else if(option !=RET_SELECT&& !g_timeIsUp){
										LCD_clearScreen();
										LCD_displayStringRowColumn(0,0,"wrong input");
										_delay_ms(500);
									}
								}

								break;

							case AIR_COND:
								while(airCond_select !=AIR_COND_RET && !g_timeIsUp){
									uint8 temp=0;
									LCD_clearScreen();
									LCD_displayStringRowColumn(0,0,"1-Set Temp");
									LCD_displayStringRowColumn(1,0,"2-Control 0-RET");
									_delay_ms(250);
									airCond_select=	KeyPad_getPressedKey();
									if(airCond_select == AIR_COND_SET_TEMP){
										LCD_clearScreen();
										LCD_displayStringRowColumn(0,0,"set temp=  C");
										for(int i=0;i<2;i++){
											_delay_ms(250);
											temp= KeyPad_getPressedKey();
											LCD_goToRowColumn(0,9+i);
											LCD_intgerToString(temp);
											temperature=temperature*10+temp;
											_delay_ms(300);


										}
										SPI_sendByte(SET_AC_TEMPERATURE);
										SPI_sendByte((uint8)temperature);
										LCD_clearScreen();
										LCD_displayStringRowColumn(0,0,"temperature sent");
										_delay_ms(500);


									}
									else if(airCond_select == AIR_COND_CNTRL){
										SPI_sendByte(CHECK_AC_CONDITION);
										sint8 pressed=-1;
										while(pressed != AIR_COND_RET && !g_timeIsUp){
											LCD_clearScreen();
											LCD_displayStringRowColumn(0,0,"AirCond S:");
											LCD_displayStringRowColumn(1,0,"1:ON 2:OFF 0:RET");
											SPI_tranceiver(SEND_STATUS_AC);
											_delay_ms(100);
											g_AC_status=SPI_tranceiver(0xFF);
											if(g_AC_status==ON_STATUS ){
												LCD_displayStringRowColumn(0,10,"ON");
											}
											else
												LCD_displayStringRowColumn(0,10,"OFF");
											_delay_ms(250);
											pressed=KeyPad_getPressedKey();
											if(pressed == ON ){
												SPI_sendByte(TURN_AC_ON );
												break;
											}
											else if(pressed == OFF){
												SPI_sendByte(TURN_AC_OFF );
												break;
											}
											else if(pressed !=RET_SELECT && !g_timeIsUp){
												LCD_clearScreen();
												LCD_displayStringRowColumn(0,0,"wrong input");
												_delay_ms(500);
											}

										}

									}

									else if(airCond_select !=AIR_COND_RET && !g_timeIsUp){
										LCD_clearScreen();
										LCD_displayStringRowColumn(0,0,"wrong choose!!");
										_delay_ms(500);
									}

								}

								break;

							default:
								if(more_modules != RET_TO_ADMIN_MENU  && !g_timeIsUp ){
									LCD_clearScreen();
									LCD_displayStringRowColumn(0,0,"invaild module!!");
									_delay_ms(500);
								}
								break;
							}


						}
						break;



					default:
						if(!g_timeIsUp){
							LCD_clearScreen();
							LCD_displayStringRowColumn(0,0,"invaild module AM");
							_delay_ms(500);
						}
						break;

					}

				}

				else if(mode == GUEST_MODE){

					LCD_displayStringRowColumn(1,0,"3:Room3 4:Room4");
					_delay_ms(500);
					module = KeyPad_getPressedKey();
					switch(module){

					case ROOM1_GUEST_MODE :
						while(option !=RET_TO_GUEST_MENU  && option != ON && option !=OFF && !g_timeIsUp){
							LCD_clearScreen();
							LCD_displayStringRowColumn(0,0,"Room1  S:");
							SPI_tranceiver(SEND_STATUS_ROOM1);
							_delay_ms(100);
							g_room1Status=SPI_tranceiver(0xFF);
							if(g_room1Status==ON_STATUS)
								LCD_displayStringRowColumn(0,8,"ON");
							else
								LCD_displayStringRowColumn(0,8,"OFF");
							LCD_displayStringRowColumn(1,0,"1:ON 2:OFF 0:RET");
							_delay_ms(500);
							option = KeyPad_getPressedKey();
							if(option == ON ){
								SPI_sendByte(TURN_ROOM1_ON );
								break;
							}
							else if(option == OFF){
								SPI_sendByte(TURN_ROOM1_OFF );
								break;
							}
							else if(option !=RET_TO_GUEST_MENU  && !g_timeIsUp){
								LCD_clearScreen();
								LCD_displayStringRowColumn(0,0,"wrong input");
								_delay_ms(500);
							}
						}

						break;


					case ROOM2_GUEST_MODE :
						while(option !=RET_TO_GUEST_MENU  && option != ON && option !=OFF && !g_timeIsUp){
							LCD_clearScreen();
							LCD_displayStringRowColumn(0,0,"Room2  S:");
							SPI_tranceiver(SEND_STATUS_ROOM2);
								_delay_ms(100);
								g_room2Status=SPI_tranceiver(0xFF);
							if( g_room2Status==ON_STATUS )
								LCD_displayStringRowColumn(0,8,"ON");
							else
								LCD_displayStringRowColumn(0,8,"OFF");
							LCD_displayStringRowColumn(1,0,"1:ON 2:OFF 0:RET");
							_delay_ms(500);
							option = KeyPad_getPressedKey();
							if(option == ON ){
								SPI_sendByte(TURN_ROOM2_ON );
								break;
							}
							else if(option == OFF){
								SPI_sendByte(TURN_ROOM2_OFF );
								break;
							}
							else if(option !=RET_TO_GUEST_MENU  && !g_timeIsUp){
								LCD_clearScreen();
								LCD_displayStringRowColumn(0,0,"wrong input");
								_delay_ms(500);
							}
						}

						break;


					case ROOM3_GUEST_MODE :
						while(option !=RET_TO_GUEST_MENU  && option != ON && option !=OFF && !g_timeIsUp){
							LCD_clearScreen();
							LCD_displayStringRowColumn(0,0,"Room3  S:");
							SPI_tranceiver(SEND_STATUS_ROOM3);
							_delay_ms(100);
							g_room3Status=SPI_tranceiver(0xFF);
							if(g_room3Status==ON_STATUS)
								LCD_displayStringRowColumn(0,8,"ON");
							else
								LCD_displayStringRowColumn(0,8,"OFF");
							LCD_displayStringRowColumn(1,0,"1:ON 2:OFF 0:RET");
							_delay_ms(500);
							option = KeyPad_getPressedKey();
							if(option == ON ){
								SPI_sendByte(TURN_ROOM3_ON );
								break;
							}
							else if(option == OFF){
								SPI_sendByte(TURN_ROOM3_OFF );
								break;
							}
							else if(option !=RET_TO_GUEST_MENU  && !g_timeIsUp){
								LCD_clearScreen();
								LCD_displayStringRowColumn(0,0,"wrong input");
								_delay_ms(500);
							}
						}

						break;


					case ROOM4_GUEST_MODE :
						while(option !=RET_TO_GUEST_MENU  && option != ON && option !=OFF && !g_timeIsUp){
							LCD_clearScreen();
							LCD_displayStringRowColumn(0,0,"Room4  S:");
							SPI_tranceiver(SEND_STATUS_ROOM4);
							_delay_ms(100);
							g_room4Status=SPI_tranceiver(0xFF);
							if(g_room4Status==ON_STATUS )
								LCD_displayStringRowColumn(0,8,"ON");
							else
								LCD_displayStringRowColumn(0,8,"OFF");
							LCD_displayStringRowColumn(1,0,"1:ON 2:OFF 0:RET");
							_delay_ms(500);
							option = KeyPad_getPressedKey();
							if(option == ON ){
								SPI_sendByte(TURN_ROOM4_ON );
								break;
							}
							else if(option == OFF){
								SPI_sendByte(TURN_ROOM4_OFF );
								break;
							}
							else if(option !=RET_TO_GUEST_MENU  && !g_timeIsUp){
								LCD_clearScreen();
								LCD_displayStringRowColumn(0,0,"wrong input");
								_delay_ms(500);
							}
						}

						break;


					default:
						if(option !=RET_TO_GUEST_MENU  && !g_timeIsUp ){
							LCD_clearScreen();
							LCD_displayStringRowColumn(0,0,"invaild module GM");
							_delay_ms(500);
						}
						break;
					}
				}
			}
		}
	}
}

