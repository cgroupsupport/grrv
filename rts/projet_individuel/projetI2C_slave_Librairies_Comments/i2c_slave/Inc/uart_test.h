#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_uart.h"

#ifndef UART_CGROUP_H
#define UART_CGROUP_H

void MX_USART2_UART_Init(void);
void SendChar (char letter);
void SendStr (char* stringToSend);
char GetChar (void);
void GetStr(char* rxString);
void terminal( char * string);

#endif
