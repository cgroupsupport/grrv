/*----------------------------------------------------------------------------*/
/**
 * \file 		i2c_test.h
 * \brief 		Header UART for UART Project, test and validation of the I2C_Slave module
 * \authors 		Cgroup & ENSI Corp
 * \version 		0.1
 * \date 		25 septembre 2015
 * \copyright 		C group : Security Department
 * \bug 		no bug
 *
 */
/*----------------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_i2c.h"

#ifndef I2C_SLAVE_CGROUP_H
#define I2C_SLAVE_CGROUP_H

/**
   * \brief Initialisation of the I2C module
   * \param None
   * \retval None
   */
void MX_I2C1_Init (void);

/**
   * \brief read as slave
   * \param None
   * \retval None
   */
void read_i2c (void);

/**
   * \brief send as slave
   * \param None
   * \retval None
   */
void send_i2c (void);

#endif

/************************ C group & ENSI Corp *********************************/
