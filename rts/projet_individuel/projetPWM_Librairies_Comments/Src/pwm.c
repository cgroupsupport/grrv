#include "stm32f4xx_hal.h"
#include "pwm.h"

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;

TIM_OC_InitTypeDef sConfigOC_PROP1;
TIM_OC_InitTypeDef sConfigOC_PROP2;
TIM_OC_InitTypeDef sConfigOC_STEER_CAM;
TIM_OC_InitTypeDef sConfigOC_LED;

/* TIM2 init function */
void MX_PWM_PROP_Init(void)
{

  TIM_MasterConfigTypeDef sMasterConfig;

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = TIM_PERIOD;
  htim2.Init.ClockDivision = 0;
  HAL_TIM_PWM_Init(&htim2);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig);

  sConfigOC_PROP1.OCMode = TIM_OCMODE_PWM1;
  sConfigOC_PROP1.Pulse = 0;
  sConfigOC_PROP1.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC_PROP1.OCFastMode = TIM_OCFAST_DISABLE;
	
	sConfigOC_PROP2.OCMode = TIM_OCMODE_PWM1;
  sConfigOC_PROP2.Pulse = 0;
  sConfigOC_PROP2.OCPolarity = TIM_OCPOLARITY_LOW;
  sConfigOC_PROP2.OCFastMode = TIM_OCFAST_DISABLE;
	
	
  HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC_PROP1, TIM_CHANNEL_1);
  HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC_PROP2, TIM_CHANNEL_3);
	
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
}

/* TIM3 init function */
void MX_PWM_STEER_CAM_Init(void)
{

  TIM_MasterConfigTypeDef sMasterConfig;

  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = TIM_PERIOD;
  htim3.Init.ClockDivision = 0;
  HAL_TIM_PWM_Init(&htim3);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig);

  sConfigOC_STEER_CAM.OCMode = TIM_OCMODE_PWM1;
  sConfigOC_STEER_CAM.Pulse = 0;
  sConfigOC_STEER_CAM.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC_STEER_CAM.OCFastMode = TIM_OCFAST_DISABLE;
	
  HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC_STEER_CAM, TIM_CHANNEL_1);
  HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC_STEER_CAM, TIM_CHANNEL_2);

	if(HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1) != HAL_OK)
		while(1){}
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
}

/* TIM4 init function */
void MX_PWM_LED_Init(void)
{

  TIM_MasterConfigTypeDef sMasterConfig;
 
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 0;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = TIM_PERIOD;
  htim4.Init.ClockDivision = 0;
  HAL_TIM_PWM_Init(&htim4);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig);

  sConfigOC_LED.OCMode = TIM_OCMODE_PWM1;
  sConfigOC_LED.Pulse = 0;
  sConfigOC_LED.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC_LED.OCFastMode = TIM_OCFAST_DISABLE;
  HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC_LED, TIM_CHANNEL_2);

	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
}

/* USER CODE BEGIN 4 */

void PWM_CH1_Pulse (int pls){
	
	sConfigOC_PROP1.Pulse = pls;
	HAL_TIM_PWM_ConfigChannel(&htim2,&sConfigOC_PROP1, TIM_CHANNEL_1);
//	if( HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC_HIGH, TIM_CHANNEL_1) !=HAL_OK){
//		/* ERROR */
//		/* Would have to put flag error to 1 for the STR */
//		while(1){} // Trap to wait for the user to push the button
//	}
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
//	if (HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1) != HAL_OK){
//		/*ERROR */
//		/* Would have to put a flag to 1 for the STR */
//		while(1){} // Trap to wait for the user to push the button
//	}
	
}

void PWM_CH2_Pulse (int pls){
	
	sConfigOC_PROP2.Pulse = pls;
	HAL_TIM_PWM_ConfigChannel(&htim2,&sConfigOC_PROP2, TIM_CHANNEL_3);
//	if( HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC_HIGH, TIM_CHANNEL_1) !=HAL_OK){
//		/* ERROR */
//		/* Would have to put flag error to 1 for the STR */
//		while(1){} // Trap to wait for the user to push the button
//	}
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
//	if (HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1) != HAL_OK){
//		/*ERROR */
//		/* Would have to put a flag to 1 for the STR */
//		while(1){} // Trap to wait for the user to push the button
//	}
	
}

void PWM_CH3_Pulse (int pls){
	
	sConfigOC_STEER_CAM.Pulse = pls;
	HAL_TIM_PWM_ConfigChannel(&htim3,&sConfigOC_STEER_CAM, TIM_CHANNEL_1);
//	if( HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC_HIGH, TIM_CHANNEL_1) !=HAL_OK){
//		/* ERROR */
//		/* Would have to put flag error to 1 for the STR */
//		while(1){} // Trap to wait for the user to push the button
//	}
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
//	if (HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1) != HAL_OK){
//		/*ERROR */
//		/* Would have to put a flag to 1 for the STR */
//		while(1){} // Trap to wait for the user to push the button
//	}
	
}

void PWM_CH4_Pulse (int pls){
	
	sConfigOC_STEER_CAM.Pulse = pls;
	HAL_TIM_PWM_ConfigChannel(&htim3,&sConfigOC_STEER_CAM, TIM_CHANNEL_2);
//	if( HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC_HIGH, TIM_CHANNEL_1) !=HAL_OK){
//		/* ERROR */
//		/* Would have to put flag error to 1 for the STR */
//		while(1){} // Trap to wait for the user to push the button
//	}
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
//	if (HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1) != HAL_OK){
//		/*ERROR */
//		/* Would have to put a flag to 1 for the STR */
//		while(1){} // Trap to wait for the user to push the button
//	}
	
}

void PWM_LED_Pulse (int pls){
	
	sConfigOC_STEER_CAM.Pulse = pls;
	HAL_TIM_PWM_ConfigChannel(&htim4,&sConfigOC_LED, TIM_CHANNEL_2);
//	if( HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC_HIGH, TIM_CHANNEL_1) !=HAL_OK){
//		/* ERROR */
//		/* Would have to put flag error to 1 for the STR */
//		while(1){} // Trap to wait for the user to push the button
//	}
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
//	if (HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1) != HAL_OK){
//		/*ERROR */
//		/* Would have to put a flag to 1 for the STR */
//		while(1){} // Trap to wait for the user to push the button
//	}
	
}

