/**
  ******************************************************************************
  * @file    adc_cgroup.h
  * @author  C group - ENSI Corp
  * @version V1.0
  * @date    01-October-2015
  * @brief   This file contains all the functions prototypes for the ADC module 
  ******************************************************************************
**/

#ifndef ADC_H
#define ADC_H

/* Functions -----------------------------------------------------------------*/

/**
   * @fn MX_ADC1_Init
   * @brief Initialize the ADC1
	 * @param None
   * @retval None
   */
void MX_ADC1_Init(void);

/**
   * @fn adc1
   * @brief Using the ADC1
   * @param None
	 * @return The value of the ADC in integer format 
	*/
int adc1(void);

#endif

/************************ C group && ENSI Corp ********************************/
