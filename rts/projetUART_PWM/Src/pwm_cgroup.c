#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_tim.h"
#include "pwm_cgroup.h"

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

TIM_OC_InitTypeDef sConfigOC_PROP1;
TIM_OC_InitTypeDef sConfigOC_PROP2;
TIM_OC_InitTypeDef sConfigOC_STEER;
TIM_OC_InitTypeDef sConfigOC_CAM;

/* TIM2 init function */
void MX_PWM_TIM2_Init(void)
{

  TIM_MasterConfigTypeDef sMasterConfig;

	//Initialisation of TIM2_CH1
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = TIM_PERIOD_PROP;
  htim2.Init.ClockDivision = 0;
  HAL_TIM_PWM_Init(&htim2);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig);

  sConfigOC_PROP1.OCMode = TIM_OCMODE_PWM1;
  sConfigOC_PROP1.Pulse = 0;
  sConfigOC_PROP1.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC_PROP1.OCFastMode = TIM_OCFAST_DISABLE;
	
  HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC_PROP1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	
	//Initialisation of TIM2_CH3
	htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = TIM_PERIOD_STEER_CAM;
  htim2.Init.ClockDivision = 0;
  HAL_TIM_PWM_Init(&htim2);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig);
	
	sConfigOC_STEER.OCMode = TIM_OCMODE_PWM1;
  sConfigOC_STEER.Pulse = 0;
  sConfigOC_STEER.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC_STEER.OCFastMode = TIM_OCFAST_DISABLE;
	
  HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC_STEER, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
}

/* TIM3 init function */
void MX_PWM_TIM3_Init(void)
{

  TIM_MasterConfigTypeDef sMasterConfig;
	
	//Initialisation of TIM3_CH1
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = TIM_PERIOD_PROP;
  htim3.Init.ClockDivision = 0;
  HAL_TIM_PWM_Init(&htim3);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig);

  sConfigOC_PROP2.OCMode = TIM_OCMODE_PWM1;
  sConfigOC_PROP2.Pulse = 0;
  sConfigOC_PROP2.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC_PROP2.OCFastMode = TIM_OCFAST_DISABLE;
	
  HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC_PROP2, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	
	//Initialisation of TIM3_CH2
	htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = TIM_PERIOD_STEER_CAM;
  htim3.Init.ClockDivision = 0;
  HAL_TIM_PWM_Init(&htim3);
	
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig);
	
	sConfigOC_CAM.OCMode = TIM_OCMODE_PWM1;
  sConfigOC_CAM.Pulse = 0;
  sConfigOC_CAM.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC_CAM.OCFastMode = TIM_OCFAST_DISABLE;
	
  HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC_CAM, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
}

/* USER CODE BEGIN 4 */

int PROPULSION_Algo(int pls){
	int ret_pls = 0;
	if( pls < 175){
		ret_pls = pls * 0.75 * 65500 / 255;
	}
	if(pls >= 175 && pls <= 254){
		ret_pls = 175 * 0.75 *65500 / 255;
	}
	if ( pls == 255){
		ret_pls = 70 * 0.75 * 65500 / 255;
	}
	return ret_pls;
}

int STEERING_Algo(int pls){
	int ret_pls = 0;
	ret_pls = 1309 + (25098 - 1309)/255 * (pls + 127);
	return ret_pls;
}	

int CAM_Algo(int pls){
	int ret_pls;
	ret_pls = 62224 - (pls + 127) * 205;
	return ret_pls;
}

void PWM_CH1_Pulse (int pls){
	
	// Recalculation of the pulse to be in our speed range
	pls = PROPULSION_Algo(pls);
	sConfigOC_PROP1.Pulse = pls;
	HAL_TIM_PWM_ConfigChannel(&htim2,&sConfigOC_PROP1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
}
void PWM_CH2_Pulse (int pls){
	
	// Recalculation of the pulse to be in our speed range
	pls = PROPULSION_Algo(pls);
	sConfigOC_PROP2.Pulse = pls;
	HAL_TIM_PWM_ConfigChannel(&htim3,&sConfigOC_PROP2, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	
}

void PWM_CH3_Pulse (int pls){
	
	// Recalculation of the pulse to be in our speed range
	pls = STEERING_Algo(pls);
	sConfigOC_STEER.Pulse = pls;
	HAL_TIM_PWM_ConfigChannel(&htim2,&sConfigOC_STEER, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
}

void PWM_CH4_Pulse (int pls){
	
	// Recalculation of the pulse to be in our speed range
	pls = CAM_Algo(pls);
	
	sConfigOC_CAM.Pulse = pls;
	HAL_TIM_PWM_ConfigChannel(&htim3,&sConfigOC_CAM, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);	
}

