/**
 ******************************************************************************
 * @file    main.c
 * @author  C group - ENSI Corp
 * @version V1.0
 * @date    02-October-2015
 * @brief   main file
 ******************************************************************************
 */


/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "pwm_cgroup.h"
#include "uart_cgroup.h"
#include "adc_cgroup.h"
#include "i2c_slave_cgroup.h"
#include "initialize_cgroup.h"

/* Private variables ---------------------------------------------------------*/
int flagMode;		/* 0 = Manual Mode || 1 = Automatic Mode */

/**
 * @fn main()
 * @brief main function for the complete project
 * @param None
 * @retval zero
 */
int main(void){

  /* MCU Configuration */
	initialization();

	/* switch to automatic mode */
	flagMode=1;

  /* Infinite loop */
  while (1){
		/* Call the program of the Earth Drone */
		i2c_cgroup_function(1);
  }
}

#ifdef USE_FULL_ASSERT

/**
 * @fn assert_failed(uint8_t* file, uint32_t line)
 * @brief Reports the name of the source file and the source line number
 * where the assert_param error has occurred.
 * @param file: pointer to the source file name
 * @param line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line){
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}

#endif

/**
  * @}
  */

/**
  * @}
*/

/************************ C group && ENSI Corp ********************************/
