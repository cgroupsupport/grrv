/*----------------------------------------------------------------------------*/
/*  Authors 	:	C group																												*/
/*	Partners 	: ENSI Corp																											*/
/*	Title			:	UART Project																									*/
/* 	File Name	: UART.h																												*/
/*----------------------------------------------------------------------------*/

#ifndef UART_CGROUP_H
#define UART_CGROUP_H

/* Private variables ---------------------------------------------------------*/
void CGROUP_HAL_UART_MspInit(UART_HandleTypeDef *huart);
void MX_USART2_UART_Init(void);
void SendChar (char letter);
void SendStr (char* stringToSend);
char GetChar (void);
void GetStr(char* rxString);
void terminal( char * string);

#endif

/************************ C group && ENSI Corp ********************************/
