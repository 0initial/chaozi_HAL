#include "timer.h"
#include "uart.h"

TIM_HandleTypeDef             tim1;

void Timer1_Init(uint16_t psc, uint16_t arr, uint8_t rep)
{
	tim1.Instance = TIM1;
	tim1.Init.Prescaler = psc - 1;
	tim1.Init.CounterMode = TIM_COUNTERMODE_UP;
	tim1.Init.Period = arr - 1;
	tim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV2;
	tim1.Init.RepetitionCounter = rep;
	tim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	HAL_TIM_Base_Init(&tim1);
	__HAL_TIM_CLEAR_FLAG(&tim1, TIM_FLAG_UPDATE);
	
	HAL_TIM_Base_Start_IT(&tim1);
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM1)
	{
		__HAL_RCC_TIM1_CLK_ENABLE();
		
		HAL_NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 1, 0); // 配置两个C中断，一个输入捕获，一个输出比较
		HAL_NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);         // 开启中断
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM1)
	{
		u1_printf("定时器3唤醒\r\n");
		HAL_TIM_Base_Stop_IT(&tim1);
		// 唤醒后再开启HAL库时基，防止HAL库程序不运行
		SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
	}
}



