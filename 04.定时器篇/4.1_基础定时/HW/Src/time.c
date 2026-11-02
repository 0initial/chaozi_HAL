#include "time.h"
#include "uart.h"

TIM_HandleTypeDef tim1;

void Timer1_Init(uint16_t psc, uint16_t arr, uint8_t rep)
{
	tim1.Instance = TIM1;
	tim1.Init.Prescaler = psc - 1;
	tim1.Init.CounterMode = TIM_COUNTERMODE_UP;
	tim1.Init.Period = arr - 1;
	tim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	tim1.Init.RepetitionCounter = rep;
	tim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	HAL_TIM_Base_Init(&tim1);
	__HAL_TIM_CLEAR_FLAG(&tim1, TIM_FLAG_UPDATE);
	HAL_TIM_Base_Start(&tim1);
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM1)
	{
		__HAL_RCC_TIM1_CLK_ENABLE();
		
		// ¿ªÆôÖÐ¶Ï
		HAL_NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
	}
}

