/*----------------------------------------------------------------------------*/
/**
 * \file 		main.c
 * \brief 		Main program for the Project with RTOS, test and validation of the final project
 * \authors 		Cgroup & ENSI Corp
 * \version 		0.1
 * \date 		02 octobre 2015
 * \copyright 		C group : Secuity Department
 * \bug 		no bug
 *
 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/

#define osObjectsPublic                     // define objects in main module

/* Includes ------------------------------------------------------------------*/
#include "osObjects.h"                      // RTOS object definitions
#include "stm32f4xx_hal.h"
#include "pwm_cgroup.h"
#include "uart_cgroup.h"
#include "adc_cgroup.h"
#include "i2c_slave_cgroup.h"
#include "thread_cgroup.h"

/* Private variables ---------------------------------------------------------*/
extern int Init_Thread(void);


/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/**
   * \brief Main function PWM PROJECT
   * \param None
   * \retval 0   
   */
int main (void) {
	
  osKernelInitialize ();                    // initialize CMSIS-RTOS

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
	MX_I2C1_Init();
	MX_ADC1_Init();
	MX_PWM_TIM2_Init();
  MX_PWM_TIM3_Init();

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);	
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6,GPIO_PIN_RESET);// generate the 5V for the enable on power board.

	Init_Thread();
	
  osKernelStart ();                         // start thread execution 
	 
  while(1);
}


/**
   * \brief Configure the System Clock
   * \param None
   * \retval None   
   */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  __PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

}

/************************ C group & ENSI Corp *********************************/

