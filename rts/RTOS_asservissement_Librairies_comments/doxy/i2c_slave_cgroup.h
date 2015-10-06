/**
 ******************************************************************************
 * @file    i2c_slave_cgroup.h
 * @author  C group - ENSI Corp
 * @version V1.0
 * @date    01-October-2015
 * @brief   This file contains all the functions prototypes for the I2C module
 ******************************************************************************
 */

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_i2c.h"

#ifndef I2C_SLAVE_CGROUP_H
#define I2C_SLAVE_CGROUP_H

/**
   * @fn i2c_cgroup_function()
   * @brief I2C1 function for complete project
   * @param None
   * @retval None
   */
void i2c_cgroup_function(void);

/**
   * @fn MX_I2C1_Init()
   * @brief I2C1 initialization function
	 * @param None
   * @retval None
   */
void MX_I2C1_Init (void);

/**
   * @fn read_i2c()
   * @brief I2C1 reading function
	 * @param None
   * @retval None
   */
void read_i2c (uint8_t*);

/**
   * @fn send_i2c()
   * @brief I2C1 sending function
	 * @param None
   * @retval None
   */
void send_i2c (void);

#endif

