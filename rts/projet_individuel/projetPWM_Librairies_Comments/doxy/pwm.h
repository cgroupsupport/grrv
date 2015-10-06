/*----------------------------------------------------------------------------*/
/**
 * \file 		pwm.h
 * \brief 		Header UART for UART Project, test and validation of the PWM module
 * \authors 		Cgroup & ENSI Corp
 * \version 		0.1
 * \date 		25 septembre 2015
 * \copyright 		C group : Secuity Department
 * \bug 		no bug
 *
 */
/*----------------------------------------------------------------------------*/

#ifndef UART_H
#define UART_H

#define TIM_PERIOD_PROP        (8400-1) //period value
#define TIM_PERIOD_STEER_CAM   (65500-1) //period value

//DUTY_CYCLE_PROP definition @10kHz
#define DUTY_CYCLE_PROP_0	0
#define DUTY_CYCLE_PROP_25   2099
#define DUTY_CYCLE_PROP_50   4199
#define DUTY_CYCLE_PROP_75   6299
#define DUTY_CYCLE_PROP_100  8399

//DUTY_CYCLE_STEER_CAM definition @1.3kHz
#define DUTY_CYCLE_STEER_CAM_1	  851
#define DUTY_CYCLE_STEER_CAM_20   13099
#define DUTY_CYCLE_STEER_CAM_25   16374
#define DUTY_CYCLE_STEER_CAM_40   26199
#define DUTY_CYCLE_STEER_CAM_50   37749
#define DUTY_CYCLE_STEER_CAM_60   39299
#define DUTY_CYCLE_STEER_CAM_80   52399
#define DUTY_CYCLE_STEER_CAM_100  65500

/**
   * \brief Initialisation of the PWM module for the propulsion of the car.
   * \param None
   * \retval None
   */
void MX_PWM_PROP_Init(void);

/**
   * \brief Initialisation of the PWM module for steering and moving the Camera of the car.
   * \param None
   * \retval None
   */
void MX_PWM_STEER_CAM_Init(void);

/**
   * \brief Change the pulse width of the channel 1 of the PWM
   * \param the pulse, it has to be chosen in the DUTY_CYCLE_PROP at the beginning of the file
   * \retval None
   */
void PWM_CH1_Pulse (int pls);

/**
   * \brief Change the pulse width of the channel 2 of the PWM
   * \param the pulse, it has to be chosen in the DUTY_CYCLE_PROP at the beginning of the file
   * \retval None
   */
void PWM_CH2_Pulse (int pls);

/**
   * \brief Change the pulse width of the channel 3 of the PWM
   * \param the pulse, it has to be chosen in the DUTY_CYCLE_STEER_CAM at the beginning of the file
   * \retval None
   */
void PWM_CH3_Pulse (int pls);

/**
   * \brief Change the pulse width of the channel 4 of the PWM
   * \param the pulse, it has to be chosen in the DUTY_CYCLE_STEER_CAM at the beginning of the file
   * \retval None
   */
void PWM_CH4_Pulse (int pls);


#endif

/************************ C group & ENSI Corp *********************************/
