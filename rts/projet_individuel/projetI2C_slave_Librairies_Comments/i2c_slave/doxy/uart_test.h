/*----------------------------------------------------------------------------*/
/**
 * \file 		uart_test.h
 * \brief 		Header UART for UART Project, test and validation of the UART module
 * \authors 		Cgroup & ENSI Corp
 * \version 		0.1
 * \date 		25 septembre 2015
 * \copyright 		C group : Security Department
 * \bug 		no bug
 *
 */
/*----------------------------------------------------------------------------*/

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_uart.h"

#ifndef UART_CGROUP_H
#define UART_CGROUP_H

/**
   * \fn MX_USART2_UART_Init()
   * \brief Configure the USART2
   * \param None
   * \retval None
   */
void MX_USART2_UART_Init(void);

/**
   * \fn SendChar (char letter)
   * \brief Display a letter in the Terminal
   * \param The letter
   * \retval None
   */
void SendChar (char letter);

/**
   * \fn SendStr (char* stringToSend)
   * \brief Display a String in the Terminal
   * \param Char Buffer
   * \retval None
   */
void SendStr (char* stringToSend);

/**
   * \fn GetChar ()
   * \brief Recover the letter wrote by the User in the Terminal
   * \param None
   * \retval the letter recovered
   */
char GetChar (void);

/**
   * \fn GetStr (char* rxString)
   * \brief Store and Display the word wrote by the User in the Terminal
   * \param The word wrote by the User
   * \retval None
   */
void GetStr(char* rxString);

/**
   * \fn terminal( char * string)
   * \brief Terminal Function, interface between the User and the NUCLEO
   * \param Word wrote by the User
   * \retval None
   */
void terminal( char * string);

#endif

/************************ C group & ENSI Corp *********************************/
