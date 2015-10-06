/*----------------------------------------------------------------------------*/
/**
 * \file 		i2c_test.c
 * \brief 		File i2c_slave_cgroup for i2c_slave Project, test and validation of the I2C_Slave module
 * \authors 		Cgroup & ENSI Corp
 * \version 		0.1
 * \date 		25 septembre 2015
 * \copyright 		C group : Secuity Department
 * \bug 		no bug
 *
 */
/*----------------------------------------------------------------------------*/#include "i2c_slave_cgroup.h"

#include "thread_cgroup.h"
#include "uart_cgroup.h"
#include "pwm_cgroup.h"
#include <stdio.h>


/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

uint8_t txBuffer[3];
uint8_t rxBuffer[3];

/**
   * \brief Initialisation of the I2C module
   * \param None
   * \retval None
	 * \fn MX_I2C1_Init()
   */
void MX_I2C1_Init(void){
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0x20 << 1;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLED;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLED;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_ENABLED;
  HAL_I2C_Init(&hi2c1);
	
}

/**
   * \brief read as slave
   * \param None
   * \retval None
	 * \fn read_I2c(uint8_t)
   */
void read_i2c(uint8_t* rxBuffer){
	if( HAL_I2C_Slave_Receive_IT(&hi2c1, rxBuffer, 3) != HAL_OK ){
	}
	while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY) {	
	}
}

/**
   * \brief send as slave
   * \param None
   * \retval None
	 * \fn send_i2c()
   */
void send_i2c (void){
	txBuffer[0] = 's';
	txBuffer[1] = 'e';
	txBuffer[2] = 'd';
	char buff[20];

	if ( HAL_I2C_Slave_Transmit_IT(&hi2c1, txBuffer, 3) != HAL_OK ){
	}
	
	while (HAL_I2C_GetState(&hi2c1) == HAL_I2C_STATE_BUSY_TX ) {	
	}
	SendStr("master wants to read \r\n");	
	sprintf(buff,"%d %d %d",rxBuffer[0],rxBuffer[1],rxBuffer[2]);
	SendStr(buff);	
	SendStr("\r\n control :# ");		
}

void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
}

/************************ C group & ENSI Corp *********************************/

