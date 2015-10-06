/*----------------------------------------------------------------------------*/
/**
 * \file 		thread_cgroup.h
 * \brief 		Header thread for the rtos project
 * \authors 		Cgroup & ENSI Corp
 * \version 		0.1
 * \date 		25 septembre 2015
 * \copyright 		C group : Secuity Department
 * \bug 		no bug
 *
 */
/*----------------------------------------------------------------------------*/

#include "cmsis_os.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include "i2c_slave_cgroup.h"

#define delay500 500
#define delay100 100
#define UART_MODE 0
#define I2C_MODE  1
#define ERROR     1
#define NO_ERROR  0

#ifndef THREAD_CGROUP_H
#define THREAD_CGROUP_H

static int flagMode=1;     /* 0 = uart mode, 1 = i2c mode */
static int flagError;      /* 0 = no error,  1 = error */

/**
   * \brief Main task of the rtos project, save the value sent by the android module in queues for the other tasks
   * \param *argument
   * \retval None
	 * \fn uart_i2c_thread(*argument)
   */
void uart_i2c_thread (void const *argument);                         /* thread main function */

/**
   * \brief task mode of the rtos project, place the project in i2c or uart mode, depends on the user choice
   * \param *argument
   * \retval None
	 * \fn mode_thread(*argument)
   */
void mode_thread (void const *argument);                             /* thread function mode */ 

/**
   * \brief task cam of the rtos project, recover the value in the camera queue, and load this value in the camera servo motor
   * \param *argument
   * \retval None
	 * \fn cam_thread(*argument)
   */
void cam_thread (void const *argument);															 /* thread function camera */

/**
   * \brief task steering of the rtos project, recover the value in the steering queue, and load this value in the steering servo motor
   * \param *argument
   * \retval None
	 * \fn steer_thread(*argument)
   */
void steer_thread (void const *argument);														 /* thread function steer */

/**
   * \brief task propulsion of the rtos project, recover the value in the propulsion queue, and load this value in the propulsion motors
   * \param *argument
   * \retval None
	 * \fn prop_thread(*argument)
   */
void prop_thread (void const *argument);														 /* thread function prop */

/**
   * \brief algorithme asservissement for the propulsion motors 
   * \param float* y_star2, float* y_star1, float* y_star0, float* Commande1, float* Commande0, float * I_mes[]
   * \retval None
	 * \fn asservissement(float* y_star2, float* y_star1, float* y_star0, float* Commande1, float* Commande0, float I_mes[])
   */
void asservissement(float* y_star2, float* y_star1, float* y_star0, float* Commande1, float* Commande0, float * I_mes[]);


static osThreadId uart_i2c_ID;                                       /* thread main id */
static osThreadId mode_ID;                                           /* thread id mode */
static osThreadId cam_ID;                                            /* thread id camera */
static osThreadId steer_ID;																					 /* thread id steering */
static osThreadId prop_ID;																					 /* thread id propulsion */
	

static osThreadDef (mode_thread, osPriorityNormal, 1  , 0);           /* thread object mode */
static osThreadDef (cam_thread, osPriorityNormal, 1, 0);            	/* thread object camera */
static osThreadDef (steer_thread, osPriorityNormal, 1, 0);						/* thread object steering */
static osThreadDef (uart_i2c_thread, osPriorityNormal , 1, 0);        /* thread main object */
static osThreadDef (prop_thread, osPriorityNormal, 1,0);							/* thread object propulsion */

/**
   * \brief Initialize all the tasks
   * \param None
   * \retval int 0
	 * \fn Init_thread()
   */
int Init_Thread(void);

/**
   * \brief Initialize all the GPIO of the project
   * \param None
   * \retval None
	 * \fn MX_GPIO_Init()
   */
void MX_GPIO_Init(void);

/**
   * \brief Blink led function
   * \param None
   * \retval None
	 * \fn blink_led()
   */
void blink_led(void);

/**
   * \brief Error led function
   * \param None
   * \retval None
	 * \fn error_led()
   */
void error_led(void);

#endif

/************************ C group & ENSI Corp ********************************/
