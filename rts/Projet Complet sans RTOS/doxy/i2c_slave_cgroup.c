/**
 ******************************************************************************
 * @file    i2c_slave_cgroup.c
 * @author  C group - ENSI Corp
 * @version V1.0
 * @date    01-October-2015
 * @brief   I2C module
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "i2c_slave_cgroup.h"
#include "pwm_cgroup.h"
#include "adc_cgroup.h"
#include "uart_cgroup.h"
#include <stdio.h>

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;
uint8_t rxBuffer[3];
//uint8_t txBuffer[3];
float I_mes[3];

/* MX_I2C1_Init */
void MX_I2C1_Init(void){

  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0x20 << 1;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLED;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLED;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLED;
  HAL_I2C_Init(&hi2c1);
}

/* i2c_cgroup_function */
void i2c_cgroup_function(int flagMode){

	/* Declaration of all the variables use in the PWM module */
	int prop_pls=0;
	int steer_pls=0;
	int cam_pls=0;

	/* Declaration of all the variables use in the ADC module */
	int adc_value;
	float voltage,intensity;

	/* Declaration of the variable use to send in the uart if needed */
	char string_save[200];

	/* We can leave the loop if we reset the flagMode */
	while(flagMode==1){

		/*Read the transmission with the Rasberry, complete the rxBuffer*/
		read_i2c();

		/*Test the first value of the rxBuffer
		if equal 1 : first smartphone -> Propulsion and Direction
		if equal 3 : second smartphone -> Camera */

		if(rxBuffer[0]== 0x01){

		  /* recover the value for the propulsion and steering channels */
			prop_pls = rxBuffer[1];
			steer_pls = (int8_t )rxBuffer[2];

			/*Test the value of the propulsion in order to know if we need to supply PWM1 or PWM2 */
			if(prop_pls==255){

				/*sending pulsation to channels of propulsion */
				PWM_CH2_Pulse (prop_pls);
				PWM_CH1_Pulse (0);
			}
			else{

				/*sending pulsation to channels of propulsion */
				PWM_CH1_Pulse (prop_pls);
				PWM_CH2_Pulse (0);
			}

			/*sending pulsation to steering channel*/
			PWM_CH3_Pulse (steer_pls);
		}
		else if(rxBuffer[0]== 0x03){

			/* recover the value for the camera channels */
			cam_pls = (int8_t )rxBuffer[1];

			/*sending pulsation to camera channel*/
			PWM_CH4_Pulse (cam_pls);
		}

		/* recover the value of the ADC Module*/
		adc_value=adc1();

		/* Calcul of the voltage with the value of the ADC */
		voltage=adc_value*3.3/255;

		/* Calcul of the intensity with the voltage */
		intensity=voltage*100/24;

		/* Possibility to display in the uart the value of the ADC Module and the value send to PWM Module
		to check them */
		sprintf(string_save," ADC value: adc1=%d, V=%f, I=%f \r\n control :# ", adc_value,voltage,intensity);
		SendStr(string_save);
		sprintf(string_save," PWM1=%d, PWM3=%d, PWM4=%d \r\n control :# ", prop_pls,steer_pls,cam_pls);
		SendStr(string_save);
	}
}

/* read_i2c */
void read_i2c(void){

	if( HAL_I2C_Slave_Receive_IT(&hi2c1, rxBuffer, 3) != HAL_OK ){
	}

	while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY) {
	}
}

/* Asservissement */
//int Asservissement(float I){
//	float Umax= 0.88;
//	float y_star[3]={0,0,0};
//	float Commande[3]={0,0,0};
//	float I_mes[2]={0,0};
//	float gain=0.24,return_value;

//	//Filtre d'adoucissement de la consigne
//	y_star[2]=1.658*y_star[1]-0.6771*y_star[0]+0.03502*(gain*I_mes[1])+0.02701*(gain*I_mes[0]); // Filtre adoucissement I_mes<->I_capteur
//
//	// Regulateur
//	Commande[2]=Commande[1]-24.7377*(gain*I)+11.638*(gain*I_mes[1])+15.3872*y_star[2]-1.8701*y_star[1]+0.0568*y_star[0];
//
//	// Saturation
//	if(Commande[2]>0){
//		if(Commande[2]>Umax)
//			Commande[2]=Umax;
//	}
//	else{
//		if(Commande[2]<(-Umax))
//			Commande[2]=-Umax;
//	}
//
//	// Circulation temporelle de la valeur des paramètres
//	I_mes[0]=I_mes[1];
//	I_mes[1]=I;
//
//	y_star[0]=y_star[1];
//	y_star[1]=y_star[2];
//
//	Commande[1]=Commande[2];
//	return_value=Commande[2];
//
//	return_value= return_value/Umax*255;
//
//	return return_value;
//}

/* send_i2c */
//void send_i2c (void){
//	txBuffer[0] = 's';
//	txBuffer[1] = 'e';
//	txBuffer[2] = 'd';
//	char buff[20];

//	if ( HAL_I2C_Slave_Transmit_IT(&hi2c1, txBuffer, 3) != HAL_OK ){
//	}
//
//	while (HAL_I2C_GetState(&hi2c1) == HAL_I2C_STATE_BUSY_TX ) {
//	}
//}

/* HAL_I2C_SlaveTxCpltCallback */
void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c){
  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
}

/* HAL_I2C_SlaveRxCpltCallback */
void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c){
	//HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
}

/************************ C group && ENSI Corp ********************************/
