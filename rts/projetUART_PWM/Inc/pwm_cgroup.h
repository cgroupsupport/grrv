/*----------------------------------------------------------------------------*/
/*  Authors 	:	C group																												*/
/*	Partners 	: ENSI Corp																											*/
/*	Title			:	PWM Project 																									*/
/* 	File Name	: pwm_cgroup																										*/
/*----------------------------------------------------------------------------*/

#ifndef PWM_CGROUP_H
#define PWM_CGROUP_H

#define TIM_PERIOD_PROP           (65500-1)  //period value //10kHZ => 8400
#define TIM_PERIOD_STEER_CAM      (65500-1) //period value

void MX_PWM_TIM2_Init(void);
void MX_PWM_TIM3_Init(void);

void PWM_CH1_Pulse (int pls);
void PWM_CH2_Pulse (int pls);
void PWM_CH3_Pulse (int pls);
void PWM_CH4_Pulse (int pls);



#endif
/************************ C group && ENSI Corp ********************************/
