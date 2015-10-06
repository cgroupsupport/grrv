/**
 ******************************************************************************
 * @file    pwm_cgroup.h
 * @author  C group - ENSI Corp
 * @version V1.0
 * @date    01-October-2015
 * @brief   This file contains all the functions prototypes for the PWM module
 ******************************************************************************
 */

#ifndef PWM_CGROUP_H
#define PWM_CGROUP_H

#define TIM_PERIOD_PROP           (65500-1)  //period value //10kHZ => 8400
#define TIM_PERIOD_STEER_CAM      (65500-1) //period value

/* Functions -----------------------------------------------------------------*/

/**
   * @fn MX_PWM_TIM2_Init()
   * @brief TIM2 initialization function
	 * @param None
   * @retval None
   */
void MX_PWM_TIM2_Init(void);


/**
   * @fn MX_PWM_TIM3_Init()
   * @brief TIM3 initialization function
	 * @param None
   * @retval None
   */
void MX_PWM_TIM3_Init(void);


/**
   * @fn PROPULSION_Algo(int pls)
   * @brief Pulse conversion function
	 * @param Initial pulse
   * @retval Pulse in the speed range
   */
int PROPULSION_Algo(int pls);


/**
   * @fn STEERING_Algo(int pls)
   * @brief Steering conversion function
	 * @param Initial pulse
   * @retval Pulse in the steering range
   */
int STEERING_Algo(int pls);


/**
   * @fn CAM_Algo(int pls)
   * @brief Camera conversion function
	 * @param Initial pulse
   * @retval Pulse in the allowed camera control range
   */
int CAM_Algo(int pls);


/**
   * @fn PWM_CH1_Pulse(int pls)
   * @brief Recalculation of the pulse to be in our speed range(channel 1)
	 * @param The pulse
   * @retval None
   */
void PWM_CH1_Pulse (int pls);


/**
   * @fn PWM_CH2_Pulse(int pls)
   * @brief Recalculation of the pulse to be in our speed range(channel 2)
	 * @param The pulse
   * @retval None
   */
void PWM_CH2_Pulse (int pls);


/**
   * @fn PWM_CH3_Pulse(int pls)
   * @brief Recalculation of the pulse to be in our speed range(channel 3)
	 * @param The pulse
   * @retval None
   */
void PWM_CH3_Pulse (int pls);


/**
   * @fn PWM_CH4_Pulse(int pls)
   * @brief Recalculation of the pulse to be in our speed range(channel 4)
	 * @param The pulse
   * @retval None
   */
void PWM_CH4_Pulse (int pls);

#endif

/************************ C group && ENSI Corp ********************************/
