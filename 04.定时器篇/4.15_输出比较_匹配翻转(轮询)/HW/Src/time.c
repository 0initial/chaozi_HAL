#include "time.h"
#include "uart.h"

TIM_HandleTypeDef        tim1;
TIM_OC_InitTypeDef       tim1_oc1;

void Timer1_Init(uint16_t psc, uint16_t arr, uint8_t rep)
{
	tim1.Instance = TIM1;
	tim1.Init.Prescaler = psc - 1;
	tim1.Init.CounterMode = TIM_COUNTERMODE_UP;
	tim1.Init.Period = arr - 1;
	tim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	tim1.Init.RepetitionCounter = rep;
	tim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	HAL_TIM_OC_Init(&tim1);
	__HAL_TIM_CLEAR_FLAG(&tim1, TIM_FLAG_UPDATE);
	
	tim1_oc1.OCMode = TIM_OCMODE_TOGGLE;
	tim1_oc1.Pulse = 20;
	tim1_oc1.OCPolarity = TIM_OCPOLARITY_HIGH;
	HAL_TIM_OC_ConfigChannel(&tim1, &tim1_oc1, TIM_CHANNEL_1);
	
	HAL_TIM_OC_Start(&tim1, TIM_CHANNEL_1);
}

void HAL_TIM_OC_MspInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	if(htim->Instance == TIM1)
	{
		__HAL_RCC_TIM1_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();
		
		// 定时器1的通道1 PA8
		GPIO_InitStruct.Pin = TIM1_CH1_PIN;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
		HAL_GPIO_Init(TIM1_CH1_PORT, &GPIO_InitStruct);
		
//		HAL_NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 1, 0);
//		HAL_NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM1)
	{
		
	}
}

