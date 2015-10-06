/*----------------------------------------------------------------------------*/
/*  Authors 	:	C group																												*/
/*	Partners 	: ENSI Corp																											*/
/*	Title			:	UART Project																									*/
/* 	File Name	: uart_cgroup.c																									*/
/*----------------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include <string.h>
#include <stdlib.h>
#include "uart_cgroup.h"
#include "pwm_cgroup.h"
#include "i2c_slave_cgroup.h"
#include "adc_cgroup.h"

/* Private variables ---------------------------------------------------------*/
char aTxBuffer[200];
char aRxBuffer[1] ;
UART_HandleTypeDef huart2;
__IO ITStatus UartReady = RESET;

/* MX_USART2_UART_Init */ 
void MX_USART2_UART_Init(void){
	
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&huart2);
  /* Initialize display for each RESET 																				*/
	SendStr("\r\n");	
	SendStr("\n\r ######## ##    ##  ######  ####                ######      ######   ########   #######  ##     ## ########  ");
	SendStr("\n\r ##       ###   ## ##    ##  ##                ##    ##    ##    ##  ##     ## ##     ## ##     ## ##     ## ");
	SendStr("\n\r ##       ####  ## ##        ##                ##          ##        ##     ## ##     ## ##     ## ##     ## ");
	SendStr("\n\r ######   ## ## ##  ######   ##     #######    ##          ##   #### ########  ##     ## ##     ## ########  ");
	SendStr("\n\r ##       ##  ####       ##  ##                ##          ##    ##  ##   ##   ##     ## ##     ## ##        ");
	SendStr("\n\r ##       ##   ### ##    ##  ##                ##    ##    ##    ##  ##    ##  ##     ## ##     ## ##        ");
	SendStr("\n\r ######## ##    ##  ######  ####                ######      ######   ##     ##  #######   #######  ##        ");
	SendStr("\n\r                                                                                                             \r");
	SendStr("\n\r ENSI corp. with C group partnership\r\n");
	SendStr(" computer interface of control module. Enter help to know the supported commands \r\n");
	SendStr("\r control :# ");
}

/* GetChar */
char GetChar (void){ 
	
  //char retChar;
	if(HAL_UART_Receive(&huart2, (uint8_t*)aRxBuffer, 1,60000)!=HAL_OK){
		while(1){
		}
	}
	HAL_UART_Transmit(&huart2, (uint8_t*)aRxBuffer, 1, 10000) ;
	
	return aRxBuffer[0];
}

/* GetStr */
void GetStr(char* string){
	
  char tmp;
  int i=0;
	
  /* Wait for data receiving and check for ENTER key ascii code 								*/
  while ( (tmp = GetChar()) != '\r')
    string[i++] = tmp;
	
  /* Replace ENTER key ascii code 																							*/
  string[i] = '\0';   
}

/* SendChar */
void SendChar (char letter){
	
	aTxBuffer[0]=letter;
	
	if(	HAL_UART_Transmit(&huart2,(uint8_t*)aTxBuffer, 1, 500)!= HAL_OK){
		while(1){
		}
	}
}

/* SendStr */
void SendStr (char* stringToSend){
	
	int i = strlen(stringToSend);
	
	strcpy((char *)aTxBuffer,stringToSend);
	if(	HAL_UART_Transmit(&huart2, (uint8_t*)aTxBuffer, i,500)!= HAL_OK){
    while(1){
    }
  }
}

/* Terminal writig */
void terminal( char * string){
	
	int retPls = 0;
	char string_save[200];
	int n;
	float m,o;
	
	if(strcmp (string,"help")==0){	
		SendStr(" \n\r");
		SendStr("                                                                               \r");
		SendStr("\r\n ***********************************************************************************");
		SendStr("\r\n **                                 help menu                                     **");
		SendStr("\r\n ***********************************************************************************");
		SendStr("\r\n                                                                                    ");
		SendStr(" \r auto        start automatic mode and stop manual mode   \r\n");
		SendStr(" \r manual      start manual mode and stop automatic mode   \r\n");
		SendStr(" \r speed	     update speed value (only in manual mode)    \r\n");
		SendStr(" \r steering    update steering value (only in manual mode) \r\n");
		SendStr(" \r camera      update position and voltage analog values for propulsion module \r\n");
		SendStr(" \r meas        print currents and voltage analog values for propulsion module \r\n");
		SendStr(" \r help        print supported commands                                             \r\n");
		SendStr("\r control:# ");
	}
	else if(strcmp (string,"auto")==0){
		SendStr("\n\r automatic mode is running\n\r");
		i2c_cgroup_function(1);
		SendStr("\r control:# ");
	}
  else if(strcmp (string,"manual")==0){	
		SendStr("\n\r manual mode is running\n\r");
		SendStr("\r control:# ");
	}
	else if(strcmp (string,"speed")==0){
		SendStr("\n\r enter a value between 0 (stop) and 254 (full speed), 255 (reverse, speed is fixed): \n\r"); 
	  char value[4];
		int i;
		GetStr(value);
		if(value[0]=='-')
			SendStr("\n\r error : failed value (0 to 255) ");
		if ( value[0] == '2'){
			if ( value[1] == '5'){
				if (value[2] == '5'){
					for(i=0;i<=3;i++){
						SendChar(value[i]);								
					}
					retPls = atoi(value);
					PWM_CH2_Pulse(retPls);
					PWM_CH1_Pulse(0);
				}
			}
		}
		else if(value[3]=='\0'){ 
			if(value[2]!='\0'){
				if(value[0] =='2'){
					if(value[1]<='5'){
						if(value[2]<='5'){
							for(i=0;i<=3;i++){
								SendChar(value[i]);								
							}
							retPls = atoi(value);
							PWM_CH1_Pulse(retPls);
							PWM_CH2_Pulse(0);
						}
						else
							SendStr("\n\r error : failed value (0 to 255) ");
					}
					else
						SendStr("\n\r error : failed value (0 to 255) ");
				}
				else if (value[0]<='2'){
					for(i=0;i<=3;i++){
						SendChar(value[i]);
					}					
					retPls = atoi(value);
					PWM_CH1_Pulse(retPls);
					PWM_CH2_Pulse(0);
				}
				else
					SendStr("\n\r error : failed value (0 to 255)");	
			}
			else
				for(i=0;i<=3;i++){
					SendChar(value[i]);
				}
				retPls = atoi(value);
				PWM_CH1_Pulse(retPls);
				PWM_CH2_Pulse(0);
		}
		else 
			SendStr("\n\r error : failed value (0 to 255) ");
			SendStr("\n\r control:# ");	
	}
	else if(strcmp (string,"steering")==0){
		SendStr("\n\r enter value between -127 (full left) and 128 (full right), 0 is the middle position : \n\r");
	  char value[5];
		int i;
		GetStr(value);
		if(value[0]=='-'){
			if(value[1]=='0')
				SendChar(value[1]);
			else if (value[3]=='\0' || value[2]=='\0'){
				for(i=0;i<=3;i++){
					SendChar(value[i]);
				}					
				retPls = atoi(value);
				PWM_CH3_Pulse(retPls);
				HAL_Delay(500);
				PWM_CH3_Pulse(65499);
				HAL_Delay(500);
			}
			else if(value[4]=='\0'){ 
				if(value[3]!='\0'){
					if(value[1] =='1'){
						if(value[2]<='2'){
							if(value[3]<='7'){
								for(i=0;i<=3;i++){
									SendChar(value[i]);
								}	
								retPls = atoi(value);
								PWM_CH3_Pulse(retPls);
								HAL_Delay(500);
								PWM_CH3_Pulse(65499);
								HAL_Delay(500);
							}
							else
								SendStr("\n\r error : failed value (-127 to 128) ");
						}
						else
							SendStr("\n\r error : failed value (-127 to 128) ");
					}
					else
						SendStr("\n\r error : failed value (-127 to 128) ");	
				}
				else
					for(i=0;i<=3;i++){
						SendChar(value[i]);
					}		
					retPls = atoi(value);
					PWM_CH3_Pulse(retPls);
					HAL_Delay(500);
					PWM_CH3_Pulse(65499);
					HAL_Delay(500);
			}	
			else {
				SendStr("\n\r error : failed value (-127 to 128) ");
			}
		}
		else{
			if(value[3]=='\0'){ 
				if(value[2]!='\0'){
					if(value[0] =='1'){
						if(value[1]<='2'){
							if(value[2]<='8'){
								for(i=0;i<=3;i++){
									SendChar(value[i]);
								}	
								retPls = atoi(value);
								PWM_CH3_Pulse(retPls);
								HAL_Delay(500);
								PWM_CH3_Pulse(65499);
								HAL_Delay(500);
							}
							else
								SendStr("\n\r error : failed value (-127 to 128) ");
						}
						else
							SendStr("\n\r error : failed value (-127 to 128) ");
					}
					else
						SendStr("\n\r error : failed value (-127 to 128) ");	
				}
				else
					for(i=0;i<=3;i++){
						SendChar(value[i]);
					}				
					retPls = atoi(value);
					PWM_CH3_Pulse(retPls);
					HAL_Delay(500);
					PWM_CH3_Pulse(65499);
					HAL_Delay(500);
			}	
			else{ 
				SendStr("\n\r error : failed value (-127 to 128) ");
			}
		}
		SendStr("\n\r control:# ");	
	}
	else if(strcmp (string,"camera")==0){
		SendStr("\n\r enter value between -127 (full left) and 128 (full right), 0 is the middle position : \n\r");
	  char value[5];
		int i;
		GetStr(value);
		if(value[0]=='-'){
			if(value[1]=='0'){
				SendChar(value[1]);
				retPls = atoi(value);
				PWM_CH4_Pulse(retPls);
				HAL_Delay(5000);
			}
			else if (value[3]=='\0' || value[2]=='\0'){
				for(i=0;i<=3;i++){
					SendChar(value[i]);
				}					
				retPls = atoi(value);
				PWM_CH4_Pulse(retPls);
				HAL_Delay(5000);
			}
			else if(value[4]=='\0'){ 
				if(value[3]!='\0'){
					if(value[1] =='1'){
						if(value[2]<='2'){
							if(value[3]<='7'){
								for(i=0;i<=3;i++){
									SendChar(value[i]);
								}								
								retPls = atoi(value);
								PWM_CH4_Pulse(retPls);
								HAL_Delay(5000);
							}
							else
								SendStr("\n\r error : failed value (-127 to 128) ");
						}
						else
							SendStr("\n\r error : failed value (-127 to 128) ");
					}
					else
						SendStr("\n\r error : failed value (-127 to 128) ");	
				}
				else{
					for(i=0;i<=3;i++){
						SendChar(value[i]);
					}				
					retPls = atoi(value);
					PWM_CH4_Pulse(retPls);
					HAL_Delay(5000);
				}
			}	
			else 
				SendStr("\n\r error : failed value (-127 to 128) ");
		}
		else{
			if(value[3]=='\0'){ 
				if(value[2]!='\0'){
					if(value[0] =='1'){
						if(value[1]<='2'){
							if(value[2]<='8'){
								for(i=0;i<=3;i++){
									SendChar(value[i]);
								}
								retPls = atoi(value);
								PWM_CH4_Pulse(retPls);
								HAL_Delay(5000);
							}
							else
								SendStr("\n\r error : failed value (-127 to 128) ");
						}
						else
							SendStr("\n\r error : failed value (-127 to 128) ");
					}
					else
						SendStr("\n\r error : failed value (-127 to 128) ");	
				}
				else{
					for(i=0;i<=3;i++){
						SendChar(value[i]);
					}	
					retPls = atoi(value);
					PWM_CH4_Pulse(retPls);
					HAL_Delay(5000);
				}
			}	
			else 
				SendStr("\n\r error : failed value (-127 to 128) ");
		}
		SendStr("\n\r control:# ");	
	}
	else if(strcmp (string,"meas")==0){
		SendStr("\n\r currents and voltage analog values for propulsion module : \n\r");
		n=adc1();
		m=n*3.3/255;
		o=m*100/24;
		sprintf(string_save," ADC value: adc1=%d, V=%f, I=%f \r\n control :# ", n,m,o);
		SendStr(string_save); 
		SendStr("\n\r control:# ");		
	}
	else{
		SendStr("\n\r ERROR : command not supported. Enter help to know the supported command   \n\r");
		SendStr("\r control:# ");
	}
}

/************************ C group && ENSI Corp ********************************/
