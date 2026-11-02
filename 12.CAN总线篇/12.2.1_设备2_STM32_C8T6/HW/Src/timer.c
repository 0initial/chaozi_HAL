#include "timer.h"
#include "key.h"

TIM_HandleTypeDef tim2;

void TIM2_Init(uint16_t psc, uint16_t arr)
{
	tim2.Instance = TIM2;
	tim2.Init.Prescaler = psc - 1;
	tim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	tim2.Init.Period = arr - 1;
	tim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	tim2.Init.RepetitionCounter = 0;
	tim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	HAL_TIM_Base_Init(&tim2);
	__HAL_TIM_CLEAR_FLAG(&tim2, TIM_FLAG_UPDATE);
	HAL_TIM_Base_Start_IT(&tim2);
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	__HAL_RCC_TIM2_CLK_ENABLE();
	
	HAL_NVIC_SetPriority(TIM2_IRQn, 1, 1);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM2)
	{
		// 执行定时器中断逻辑
		Key_Tick();
	}
}





