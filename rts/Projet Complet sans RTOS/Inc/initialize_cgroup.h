/**
  ******************************************************************************
  * @file    initialize_cgroup.h	
  * @author  C group - ENSI Corp
  * @version V1.0
  * @date    01-October-2015
  * @brief   This file contains all the functions prototypes for the 
	* intialization functions
  ******************************************************************************
**/

#ifndef initialize_CGROUP_H
#define initialize_CGROUP_H

/* functions -----------------------------------------------------------------*/
/**
   * @fn initialization
   * @brief initialization function for the complete project
	 * @param None
   * @retval None
   */
void initialization(void);

/**
   * @fn SystemClock_Config
   * @brief System Clock Configuration
	 * @param None
   * @retval None
   */
void SystemClock_Config(void);

/**
   * @fn MX_GPIO_Init
   * @brief Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
	 * @param None
   * @retval None
   */
static void MX_GPIO_Init(void);

#endif

/************************ C group && ENSI Corp ********************************/
