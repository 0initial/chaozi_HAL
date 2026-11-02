#include "time.h"
#include "uart.h"

TIM_HandleTypeDef        tim1;
DMA_HandleTypeDef        tim1_dmaup;
TIM_ClockConfigTypeDef   tim1_clock;

void Timer1_Init(uint16_t psc, uint16_t arr, uint8_t rep)
{
	tim1.Instance = TIM1;
	tim1.Init.Prescaler = psc - 1;
	tim1.Init.CounterMode = TIM_COUNTERMODE_UP;
	tim1.Init.Period = arr - 1;
	tim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	tim1.Init.RepetitionCounter = rep;
	tim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	HAL_TIM_Base_Init(&tim1);
	__HAL_TIM_CLEAR_FLAG(&tim1, TIM_FLAG_UPDATE);
	
	 // ED双边沿检测
//	tim1_clock.ClockSource = TIM_CLOCKSOURCE_TI1ED;
//	HAL_TIM_ConfigClockSource(&tim1, &tim1_clock);
	
	tim1_clock.ClockSource = TIM_CLOCKSOURCE_ETRMODE1;
	tim1_clock.ClockPolarity = TIM_CLOCKPOLARITY_NONINVERTED;
	tim1_clock.ClockPrescaler = TIM_CLOCKPRESCALER_DIV1;
	tim1_clock.ClockFilter = 0x03;
	HAL_TIM_ConfigClockSource(&tim1, &tim1_clock);
	HAL_TIM_Base_Start(&tim1);
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	if(htim->Instance == TIM1)
	{
		__HAL_RCC_TIM1_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();
		
		// 定时器1的ETR通道 PA12
		GPIO_InitStruct.Pin = GPIO_PIN_12;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_PULLDOWN;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM1)
	{
		u1_printf("定时器1DMA中断\r\n");
	}
}

