/*----------------------------------------------------------------------------*/
/**
 * \file 		pwm_cgroup.h
 * \brief 		Header pwm for pwm Project, test and validation of the PWM module
 * \authors 		Cgroup & ENSI Corp
 * \version 		0.1
 * \date 		25 septembre 2015
 * \copyright 		C group : Secuity Department
 * \bug 		no bug
 *
 */
/*----------------------------------------------------------------------------*/

#ifndef PWM_CGROUP_H
#define PWM_CGROUP_H

#define TIM_PERIOD_PROP           (65500-1)  //period value //10kHZ => 8400
#define TIM_PERIOD_STEER_CAM      (65500-1) //period value

/**
   * \brief Initialisation of the PWM module for the propulsion of the car.
   * \param None
   * \retval None
	 * \fn MX_PWM_TIM2_Init()
   */
void MX_PWM_TIM2_Init(void);

/**
   * \brief Initialisation of the PWM module for steering and moving the Camera of the car.
   * \param None
   * \retval None
	 * \fn MX_PWM3_Init()
   */
void MX_PWM_TIM3_Init(void);

/**
   * \brief Calcul the good propulsion to send to the propulsion motors
   * \param the propulsion sent by the user/android
   * \retval the new propulsion to put on the propulsion motors
	 * \fn int PROPULSION_Algo(int)
   */
int PROPULSION_Algo(int pls);

/**
   * \brief Calcul the good value to send to the steering servo motor
   * \param the value sent by the user/android
   * \retval the new value to put on the steering servo motor
	 * \fn int STEERING_Algo(int)
   */
int STEERING_Algo(int pls);

/**
   * \brief Calcul the good value to send to the camera servo motor
   * \param the value sent by the user/android
   * \retval the new value to put on the camera servo motor
	 * \fn int CAM_Algo(int)
   */
int CAM_Algo(int pls);

/**
   * \brief Change the pulse width of the channel 1 of the PWM
   * \param the pulse, it has to be chosen in the DUTY_CYCLE_PROP at the beginning of the file
   * \retval None
	 * \fn PWM_CH1_Pulse
   */
void PWM_CH1_Pulse (int pls);

/**
   * \brief Change the pulse width of the channel 2 of the PWM
   * \param the pulse, it has to be chosen in the DUTY_CYCLE_PROP at the beginning of the file
   * \retval None
	 * \fn PWM_CH2_Pulse
   */
void PWM_CH2_Pulse (int pls);

/**
   * \brief Change the pulse width of the channel 3 of the PWM
   * \param the pulse, it has to be chosen in the DUTY_CYCLE_STEER_CAM at the beginning of the file
   * \retval None
	 * \fn PWM_CH3_Pulse
   */
void PWM_CH3_Pulse (int pls);

/**
   * \brief Change the pulse width of the channel 4 of the PWM
   * \param the pulse, it has to be chosen in the DUTY_CYCLE_STEER_CAM at the beginning of the file
   * \retval None
	 * \fn PWM_CH4_Pulse
   */
void PWM_CH4_Pulse (int pls);



#endif
/************************ C group & ENSI Corp ********************************/
