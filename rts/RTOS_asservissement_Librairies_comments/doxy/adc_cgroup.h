/*----------------------------------------------------------------------------*/
/**
 * \file 		adc_cgroup.h
 * \brief 		Header adc for adc Project, test and validation of the ADC module
 * \authors 		Cgroup & ENSI Corp
 * \version 		0.1
 * \date 		25 septembre 2015
 * \copyright 		C group : Secuity Department
 * \bug 		no bug
 *
 */
/*----------------------------------------------------------------------------*/

#ifndef ADC_H
#define ADC_H

/* Function prototypes -------------------------------------------------------*/

/**
   * \brief Initialize the ADC1
	 * \param None
   * \retval None
	 * \fn MX_ADC1_Init()
   */
void MX_ADC1_Init(void);

/**
   * \brief Initialize the ADC1
   * \param None
	 * \return The value of the ADC in integer format
	 * \fn in adc1()
	*/
int adc1(void);

#endif

/************************ C group & ENSI Corp ********************************/
