/*----------------------------------------------------------------------------*/
/**
 * \file 		adc_cgroup.c
 * \brief 		File adc for adc Project, test and validation of the ADC module
 * \authors 		Cgroup & ENSI Corp
 * \version 		0.1
 * \date 		25 septembre 2015
 * \copyright 		C group : Secuity Department
 * \bug 		no bug
 *
 */
/*----------------------------------------------------------------------------*/

#include "stm32f4xx_hal.h"
#include "adc_cgroup.h"

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
ADC_ChannelConfTypeDef sConfig;

void MX_ADC1_Init(void){
  ADC_ChannelConfTypeDef sConfig;
/*Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)*/
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION8b;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = EOC_SINGLE_CONV;
  HAL_ADC_Init(&hadc1);
/*Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.*/
  sConfig.Channel = ADC_CHANNEL_10;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);
}

int adc1(void){
  sConfig.Channel = ADC_CHANNEL_10; 
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK){
    while(1){
		}
  }
  if (HAL_ADC_Start(&hadc1) != HAL_OK){
    while(1){
		}
  }	
	return HAL_ADC_GetValue(&hadc1);
}

/************************ C group & ENSI Corp ********************************/
