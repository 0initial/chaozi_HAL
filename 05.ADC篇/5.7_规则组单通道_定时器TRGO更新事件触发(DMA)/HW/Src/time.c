#include "time.h"
#include "uart.h"

TIM_HandleTypeDef             tim1;

TIM_HandleTypeDef             tim3;
TIM_MasterConfigTypeDef       tim3_master;

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

void Timer3_Init(uint16_t psc, uint16_t arr)
{
	tim3.Instance = TIM3;
	tim3.Init.Prescaler = psc - 1;
	tim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	tim3.Init.Period = arr - 1;
	tim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV2;
	tim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	HAL_TIM_Base_Init(&tim3);
	__HAL_TIM_CLEAR_FLAG(&tim3, TIM_FLAG_UPDATE);
	
	tim3_master.MasterOutputTrigger = TIM_TRGO_UPDATE;          // 主定时器更新时，发送信号
	HAL_TIMEx_MasterConfigSynchronization(&tim3, &tim3_master); // 配置主模式
	
	HAL_TIM_Base_Start_IT(&tim3);
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM1)
	{
		__HAL_RCC_TIM1_CLK_ENABLE();
		
		HAL_NVIC_SetPriority(TIM1_CC_IRQn, 1, 0); // 配置两个C中断，一个输入捕获，一个输出比较
		HAL_NVIC_EnableIRQ(TIM1_CC_IRQn);         // 开启中断
	}
	else if(htim->Instance == TIM3)
	{
		__HAL_RCC_TIM3_CLK_ENABLE();
		
		HAL_NVIC_SetPriority(TIM3_IRQn, 1, 0); // 配置定时器3的中断优先级
		HAL_NVIC_EnableIRQ(TIM3_IRQn);         // 开启中断
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM3)
	{
//		u1_printf("定时器3更新中断\r\n");
	}
}

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM1)
	{
		u1_printf("定时器1通道1匹配成功\r\n");
	}
}




