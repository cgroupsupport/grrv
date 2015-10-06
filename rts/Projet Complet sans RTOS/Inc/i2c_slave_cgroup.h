/**
  ******************************************************************************
  * @file    i2c_slave_cgroup.h
  * @author  C group - ENSI Corp
  * @version V1.0
  * @date    01-October-2015
  * @brief   This file contains all the functions prototypes for the I2C module 
  ******************************************************************************
**/

#ifndef I2C_SLAVE_CGROUP_H
#define I2C_SLAVE_CGROUP_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_i2c.h"

/* Functions -----------------------------------------------------------------*/

/**
   * @fn MX_I2C1_Init
   * @brief I2C1 initialization function
	 * @param None
   * @retval None
   */
void MX_I2C1_Init (void);


/**
   * @fn Asservissement
   * @brief Asservissement algorithm 
	 * @param None
   * @retval None
   */ 
//int Asservissement(float I);


/**
   * @fn read_i2c
   * @brief I2C1 reading function
	 * @param None
   * @retval None
   */	 
void read_i2c (void);


/**
   * @fn send_i2c
   * @brief I2C1 sending function
	 * @param None
   * @retval None
   */
//void send_i2c (void);


/**
   * @fn i2c_cgroup_function
   * @brief I2C1 function for complete project
	 * @param Flag
   * @retval None
   */
void i2c_cgroup_function(int flagMode);


/**
   * @fn HAL_I2C_SlaveTxCpltCallback
   * @brief I2C1 testing in transmission mode
	 * @param I2C handle
   * @retval None
   */
void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c);


/**
   * @fn HAL_I2C_SlaveRxCpltCallback
   * @brief I2C1 testing in reception mode
	 * @param I2C handle
   * @retval None
   */ 
void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c);

#endif

/************************ C group && ENSI Corp ********************************/
