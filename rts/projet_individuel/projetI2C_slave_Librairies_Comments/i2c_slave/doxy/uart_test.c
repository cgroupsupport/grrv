/*----------------------------------------------------------------------------*/
/**
 * \file 		uart_test.c
 * \brief 		File UART for UART Project, test and validation of the UART module
 * \authors 		Cgroup & ENSI Corp
 * \version 		0.1
 * \date 		25 septembre 2015
 * \copyright 		C group : Security Department
 * \bug 		no bug
 *
 */
/*----------------------------------------------------------------------------*/
#include "uart_test.h"
#include "stm32f4xx_hal_uart.h"
#include <string.h>

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;
char aTxBuffer[200];
char aRxBuffer[1] ;

/* USART2 init function */
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
  // initilisation de l'affichage à chaque Reset	
  SendStr("\n\r UART ");
  SendStr("\r\n control :# ");

}

char GetChar (void){ 
  if(	HAL_UART_Receive(&huart2, (uint8_t*)aRxBuffer, 1,10000)!= HAL_OK){
    while(1){
    }
  }	
  HAL_UART_Transmit(&huart2, (uint8_t*)aRxBuffer, 1, 10000) ;
  return aRxBuffer[0];	
}

void GetStr(char* string){
  char tmp;
  int i=0;
  // wait for data receiving and check for ENTER key ascii code
  while ( (tmp = GetChar()) != '\r')
    string[i++] = tmp;
    // replace ENTER key ascii code
    string[i] = '\0';

}

void SendChar (char letter){
  aTxBuffer[0]=letter;
  if(	HAL_UART_Transmit(&huart2,(uint8_t*)aTxBuffer, 1, 500)!= HAL_OK){
    while(1){
    }
  }
}

void SendStr (char* stringToSend){
  int i = strlen(stringToSend);
  int n = 0;
  strcpy((char *)aTxBuffer,stringToSend);
  if(	HAL_UART_Transmit(&huart2, (uint8_t*)aTxBuffer, i, 500)!= HAL_OK){
    while(1){
      if( stringToSend[n]=='\0' ) break;
      n++;
    }
  }
}

void terminal( char * string){
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
		SendStr("\r control:# ");
	}
  else if(strcmp (string,"manual")==0){
		SendStr("\n\r manual mode is running\n\r");
		SendStr("\r control:# ");
	}
	else if(strcmp (string,"speed")==0){
		SendStr("\n\r enter a value between 0 (stop) and 255 (full speed): \n\r"); 	
	  char value[4];
		int i;
		GetStr(value);
		if(value[0]=='-')
			SendStr("\n\r error : failed value (0 to 255) ");
		else if(value[3]=='\0'){ 
			if(value[2]!='\0'){
				if(value[0] =='2'){
					if(value[1]<='5'){
						if(value[2]<='5'){
							for(i=0;i<=3;i++){
								SendChar(value[i]);
							}								
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
				}
				else
					SendStr("\n\r error : failed value (0 to 255)");	
			}
			else
				for(i=0;i<=3;i++){
					SendChar(value[i]);
				}				
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
			}
			else if(value[4]=='\0'){ 
				if(value[3]!='\0'){
					if(value[1] =='1'){
						if(value[2]<='2'){
							if(value[3]<='7'){
								for(i=0;i<=3;i++){
									SendChar(value[i]);
								}								
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
			}	
			else 
				SendStr("\n\r error : failed value (-127 to 128) ");
			SendStr("\n\r control:# ");	
			
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
			}	
			else 
				SendStr("\n\r error : failed value (-127 to 128) ");
			SendStr("\n\r control:# ");	
		}
	}
	else if(strcmp (string,"camera")==0){
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
			}
			else if(value[4]=='\0'){ 
				if(value[3]!='\0'){
					if(value[1] =='1'){
						if(value[2]<='2'){
							if(value[3]<='7'){
								for(i=0;i<=3;i++){
									SendChar(value[i]);
								}								
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
			}	
			else 
				SendStr("\n\r error : failed value (-127 to 128) ");
			SendStr("\n\r control:# ");	
			
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
			}	
			else 
				SendStr("\n\r error : failed value (-127 to 128) ");
			SendStr("\n\r control:# ");	
		}
	}
	else if(strcmp (string,"meas")==0){
		SendStr("\n\r currents and voltage analog values for propulsion module : \n\r");
		SendStr("\r NOT FINISH YET \r");	
			SendStr("\n\r control:# ");		
	}
	else{
		SendStr("\n\r ERROR : command not supported. Enter help to know the supported command   \n\r");
			SendStr("\r control:# ");
	}
}

/************************ C group & ENSI Corp *********************************/
