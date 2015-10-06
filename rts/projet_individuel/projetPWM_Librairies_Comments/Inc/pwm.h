#ifndef UART_H
#define UART_H

#define TIM_PERIOD      (8400-1) //period value
#define DUTY_CYCLE_0		0
#define DUTY_CYCLE_25   2099
#define DUTY_CYCLE_50   4199
#define DUTY_CYCLE_75   6299
#define DUTY_CYCLE_100  8399

void MX_PWM_PROP_Init(void);
void MX_PWM_STEER_CAM_Init(void);
void MX_PWM_LED_Init(void);

void PWM_CH1_Pulse (int pls);
void PWM_CH2_Pulse (int pls);
void PWM_CH3_Pulse (int pls);
void PWM_CH4_Pulse (int pls);
void PWM_LED_Pulse (int pls);



#endif

