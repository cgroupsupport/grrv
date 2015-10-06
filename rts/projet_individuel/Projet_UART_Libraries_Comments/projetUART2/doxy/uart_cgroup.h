/*----------------------------------------------------------------------------*/
/**
 * \file 		uart_cgroup.h
 * \brief 		Header UART for UART Project, test and validation of the UART module
 * \authors 	Cgroup & ENSI Corp
 * \version 	0.1
 * \date 		23 septembre 2015
 * \copyright 	C group : Secuity Department
 * \bug 		no bug
 *
 */
/*----------------------------------------------------------------------------*/

#ifndef UART_CGROUP_H
#define UART_CGROUP_H

/* Private variables ---------------------------------------------------------*/
void CGROUP_HAL_UART_MspInit(UART_HandleTypeDef *huart);

/**
 * \fn MX_USART2_UART_Init()
 * \brief Configure the USART2
 * \param None
 * \retval None
 */
void MX_USART2_UART_Init(void);

/**
 * \fn SendChar()
 * \brief Display a letter in the Terminal
 * \param The letter
 * \retval None
 */
void SendChar (char letter);

/**
 * \fn SendStr()
 * \brief Display a String in the Terminal
 * \param Char Buffer
 * \retval None
 */
void SendStr (char* stringToSend);

/**
 * \fn GetChar()
 * \brief Recover the letter wrote by the User in the Terminal
 * \param None
 * \retval the letter recovered
 */
char GetChar (void);

/**
 * \fn GetStr()
 * \brief Store and Display the word wrote by the User in the Terminal
 * \param The word wrote by the User
 * \retval None
 */
void GetStr(char* rxString);

/**
 * \fn terminal()
 * \brief Terminal Function, interface between the User and the NUCLEO
 * \param Word wrote by the User
 * \retval None
 */
void terminal( char * string);

#endif

/************************ C group & ENSI Corp *********************************/
