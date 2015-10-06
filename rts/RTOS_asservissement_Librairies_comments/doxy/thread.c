/*----------------------------------------------------------------------------*/
/**
 * \file 		thread.c
 * \brief 		File thread for the rtos project
 * \authors 		Cgroup & ENSI Corp
 * \version 		0.1
 * \date 		02 octobre 2015
 * \copyright 		C group : Secuity Department
 * \bug 		no bug
 *
 */
 /*----------------------------------------------------------------------------*/

#include "pwm_cgroup.h"
#include "thread_cgroup.h"                                         
#include "uart_cgroup.h" 
#include "pwm_cgroup.h"
#include "adc_cgroup.h"
#include <string.h>
#include <stdlib.h>

#define Umax 3.52
#define T_BUFF 50

float	  gain=0.24;

uint8_t buffer[3];

/* allocate the memory resources for steerQ */
osMessageQId steerQ;
osMessageQDef (steerQ,10,int);
/* allow you to retrieve data from the steerQ */
osEvent result1;

/* allocate the memory resources for camQ */
osMessageQId camQ;
osMessageQDef (camQ,10,int);
/* allow you to retrieve data from the camQ */
osEvent result2; 

/* allocate the memory resources for propQ */
osMessageQId propQ;																							
osMessageQDef (propQ,10,int); 																		
/* allow you to retrieve data from the propQ */
osEvent result3;

/* Declaration of asservissement values */
float* Commande1;
float* Commande0;
float* y_star2;
float* y_star1; 
float* y_star0;
float* I_mes[3];
	
int Init_Thread (void) {
	/* initialize all the Queues */
	steerQ = osMessageCreate(osMessageQ(steerQ),NULL);
	camQ   = osMessageCreate(osMessageQ(camQ),NULL);
	propQ  = osMessageCreate(osMessageQ(propQ),NULL);
	
	mode_ID     = osThreadCreate (osThread(mode_thread), NULL);
	cam_ID      = osThreadCreate (osThread(cam_thread), NULL);
	steer_ID    = osThreadCreate (osThread(steer_thread), NULL);
  uart_i2c_ID = osThreadCreate (osThread(uart_i2c_thread), NULL);
	prop_ID     = osThreadCreate (osThread(prop_thread), NULL);
	
	/* init propulsion thread */
  if (!uart_i2c_ID) return(-1);
	if (!mode_ID)     return(-1);
  if (!cam_ID)      return(-1);
	if(!steer_ID)     return(-1);
	if (!prop_ID)     return(-1);
	
	/* Initialization of asservissement values */
	Commande1 = (float*)malloc(sizeof(float));
	Commande0 = (float*)malloc(sizeof(float));
	y_star2 = (float*)malloc(sizeof(float));
	y_star1 = (float*)malloc(sizeof(float));
	y_star0 = (float*)malloc(sizeof(float));
	I_mes[0] = (float*)malloc(sizeof(float));
	I_mes[1] = (float*)malloc(sizeof(float));
	I_mes[2] = (float*)malloc(sizeof(float));
	*Commande0 = 0;
	*Commande1 = 0;
	*y_star2 = 0;
	*y_star1 = 0;
	*y_star0 = 0;
	*I_mes[0] = 0;
	*I_mes[1] = 0;
	*I_mes[2] = 0;
		

  return(0);
}

void uart_i2c_thread (void const *argument) {

  while (1) {
		if(flagMode==I2C_MODE){
			/* i2c Mode */
			read_i2c(buffer);
			/*Test the first value of the rxBuffer
			if equal 1 : first smartphone -> Propulsion and Direction 
			if equal 3 : second smartphone -> Camera */
			if(buffer[0]== 0x01){
				/* copy the value for the propulsion and steering channels */
				osMessagePut(propQ,buffer[1],	50);
				osMessagePut(steerQ,(int8_t )buffer[2], 50);
			}
			else if(buffer[0]== 0x03)
				/* copy the value for the camera channels */
				osMessagePut(camQ,(int8_t )buffer[1],50);
			else
				SendStr("ERROR");
		}
		else if(flagMode==UART_MODE){
			/* uart Mode */
		}
		else{
			/* flag error */
		}
    osThreadYield();		/* suspend thread */
  }
}

void cam_thread (void const *argument) {
	/* Declaration for all the variables */
	int cam_pls=0;
	int  mcam;
  osEvent  evt;                                       
	
	while(1) {
		evt = osMessageGet(camQ, 50);  /* wait for message */	
    if (evt.status == osEventMessage) {
			mcam = evt.value.v;
			/* recover the value for the camera channels */
			cam_pls = (int8_t)mcam;
			/*sending pulsation to camera channel*/
			PWM_CH4_Pulse (cam_pls);
		}
		osThreadYield();		/* suspend thread */
	}
}

void steer_thread (void const *argument) {
	/* Declaration for all the variables */
	int steer_pls = 0;
	int msteer;
  osEvent  evt;                                          
	
	while(1){
		evt = osMessageGet(steerQ, 50);  /* wait for message */			
    if (evt.status == osEventMessage) {
			msteer = evt.value.v;
			/* recover the value for the steer channels */
			steer_pls = (int8_t )msteer;
			/* sending pulsation to steering channel */
			PWM_CH3_Pulse (steer_pls);
		}
		osThreadYield();		/* suspend thread */
	}
}

void mode_thread (void const *argument) {
	uint32_t delay;	
  while (1) {
		if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET){
			if(flagMode == I2C_MODE)
				flagMode = UART_MODE;	
			else
				flagMode = I2C_MODE;
		}
		
		if(flagMode == I2C_MODE)
			delay = delay500;
		else
			delay = delay100;
		osDelay(delay);
		if(flagError == ERROR)
			error_led();
		else
			blink_led();
		
		osDelay(delay);
    osThreadYield();	/* suspend thread */
  }
}

void prop_thread (void const *argument) {
	int prop_pls = 0;
	int mprop;
	int n;
	float volt,intensity;
	osEvent  evt;	
	
	while(1) {
		evt = osMessageGet(propQ, 50);  /* wait for message */	
		if (evt.status == osEventMessage) {
			mprop = evt.value.v;
			/* recover the value for the camera channels */
			prop_pls = mprop;
			
			/*sending pulsation to camera channel*/
			n=adc1();
			volt=n*3.3/255;
			intensity=volt*100/24;
			*I_mes[2]=intensity;
			asservissement(y_star2, y_star1, y_star0, Commande1, Commande0, I_mes);
			prop_pls = (int)(*Commande1/3.52*255+0.5);
			
			if(prop_pls==255){
				PWM_CH1_Pulse(0);
				PWM_CH2_Pulse (prop_pls);
			}
			else{
				PWM_CH1_Pulse(prop_pls);
				PWM_CH2_Pulse (0);
			}
		}
		osThreadYield();               
	}
}	

void asservissement(float* y_star2, float* y_star1, float* y_star0, float* Commande1, float* Commande0, float* I_mes[]){

	/* Saturation + initialisation */

	/****************************************** Algo implementation **********************************/

	/* Filtre d'adoucissement de la consigne */
    *y_star2=1.658*(*y_star1)-0.6771*(*y_star0)+0.03502*(gain*(*I_mes[1]))+0.02701*(gain*(*I_mes[0])); /* Filtre adoucissement I_mes<->I_capteur */

	/* Regulation*/
    *Commande1=*Commande0+3.7107*(gain*(*I_mes[2]))-1.6746*(gain*(*I_mes[1]))+2.3081*(*y_star2)-0.2805*(*y_star1)+0.0085*(*y_star0);

	/* Saturation */
	if(*Commande1>Umax) *Commande1=Umax;

  /* Circulation temporelle de la valeur des paramètres */
	*I_mes[0]=*I_mes[1];
	*I_mes[1]=*I_mes[2];

  *y_star0=*y_star1;
	*y_star1=*y_star2;

	*Commande0=*Commande1;
}


/*----------------------------------------------------------------------------
 *      INITILISATIONS
 *---------------------------------------------------------------------------*/


/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __GPIOC_CLK_ENABLE();
  __GPIOH_CLK_ENABLE();
  __GPIOA_CLK_ENABLE();
  __GPIOB_CLK_ENABLE();

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PA5 */
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	/*Configure GPIO pin : PA6 */
  GPIO_InitStruct.Pin = GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PB7 */
  GPIO_InitStruct.Pin = GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void error_led(void){
	
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
	while(1){} // waiting for user to press reset button
}

void blink_led(void){
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
	HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);
}

/************************ C group & ENSI Corp ********************************/
