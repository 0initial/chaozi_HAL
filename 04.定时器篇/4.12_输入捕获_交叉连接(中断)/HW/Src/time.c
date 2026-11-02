#include "time.h"
#include "uart.h"

TIM_HandleTypeDef        tim1;
DMA_HandleTypeDef        tim1_dmaup;
TIM_ClockConfigTypeDef   tim1_clock;
TIM_IC_InitTypeDef       tim1_ic1;
TIM_IC_InitTypeDef       tim1_ic2;

uint16_t count = 0;

void Timer1_Init(uint16_t psc, uint16_t arr, uint8_t rep)
{
	tim1.Instance = TIM1;
	tim1.Init.Prescaler = psc - 1;
	tim1.Init.CounterMode = TIM_COUNTERMODE_UP;
	tim1.Init.Period = arr - 1;
	tim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	tim1.Init.RepetitionCounter = rep;
	tim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	HAL_TIM_IC_Init(&tim1);
	__HAL_TIM_CLEAR_FLAG(&tim1, TIM_FLAG_UPDATE);
	
	tim1_ic1.ICPolarity = TIM_ICPOLARITY_RISING;
	tim1_ic1.ICSelection = TIM_ICSELECTION_INDIRECTTI;
	tim1_ic1.ICPrescaler = TIM_ICPSC_DIV1;
	tim1_ic1.ICFilter = 0x08;
	HAL_TIM_IC_ConfigChannel(&tim1, &tim1_ic1, TIM_CHANNEL_1);
	
	tim1_ic2.ICPolarity = TIM_ICPOLARITY_FALLING;
	tim1_ic2.ICSelection = TIM_ICSELECTION_INDIRECTTI;
	tim1_ic2.ICPrescaler = TIM_ICPSC_DIV1;
	tim1_ic2.ICFilter = 0x08;
	HAL_TIM_IC_ConfigChannel(&tim1, &tim1_ic2, TIM_CHANNEL_2);
	
	__HAL_TIM_ENABLE_IT(&tim1, TIM_IT_UPDATE);
	HAL_TIM_IC_Start_IT(&tim1, TIM_CHANNEL_1);
	HAL_TIM_IC_Start_IT(&tim1, TIM_CHANNEL_2);
}

void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	if(htim->Instance == TIM1)
	{
		__HAL_RCC_TIM1_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();
		
		// 定时器1的通道1 PA8
		GPIO_InitStruct.Pin = TIM1_CH1_PIN;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_PULLDOWN;
		HAL_GPIO_Init(TIM1_CH1_PORT, &GPIO_InitStruct);
		
		// 定时器1的通道2 PA9
		GPIO_InitStruct.Pin = TIM1_CH2_PIN;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		HAL_GPIO_Init(TIM1_CH2_PORT, &GPIO_InitStruct);
		
		HAL_NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
		
		HAL_NVIC_SetPriority(TIM1_CC_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(TIM1_CC_IRQn);
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM1)
	{
		u1_printf("定时器1更新事件，count：%d\r\n", ++count);
	}
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM1)
	{
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
		{
			u1_printf("定时器1通道1捕获：%d\r\n", HAL_TIM_ReadCapturedValue(&tim1, TIM_CHANNEL_1) + (count * 65536));
		}
		else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
		{
			u1_printf("定时器1通道2捕获：%d\r\n", HAL_TIM_ReadCapturedValue(&tim1, TIM_CHANNEL_2) + (count * 65536));
		}
	}
}

