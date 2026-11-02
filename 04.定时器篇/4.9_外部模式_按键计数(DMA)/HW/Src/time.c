#include "time.h"
#include "uart.h"

TIM_HandleTypeDef        tim1;
DMA_HandleTypeDef        tim1_dmaup;
TIM_ClockConfigTypeDef   tim1_clock;

uint16_t tim1_dmabuff[4] = {6-1, 7-1, 8-1, 9-1};

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
	
	// 上升沿触发 按下计数
	tim1_clock.ClockSource = TIM_CLOCKSOURCE_ETRMODE1;
	tim1_clock.ClockPolarity = TIM_CLOCKPOLARITY_NONINVERTED;
	tim1_clock.ClockPrescaler = TIM_CLOCKPRESCALER_DIV1;
	tim1_clock.ClockFilter = 0x03;
	HAL_TIM_ConfigClockSource(&tim1, &tim1_clock);
	
	// 这里Start_IT之开启了UP_DATE，所以手动开启定时器触发中断
	__HAL_TIM_ENABLE_IT(&tim1, TIM_IT_UPDATE | TIM_IT_TRIGGER);
	HAL_TIM_Base_Start_DMA(&tim1, (uint32_t *)tim1_dmabuff, 4);
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	if(htim->Instance == TIM1)
	{
		__HAL_RCC_TIM1_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_DMA1_CLK_ENABLE();
		
		// 定时器1的ETR通道 PA12
		GPIO_InitStruct.Pin = TIM1_ETR_PIN;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_PULLDOWN;
		HAL_GPIO_Init(TIM1_ETR_PORT, &GPIO_InitStruct);
		
		tim1_dmaup.Instance = DMA1_Channel5;
		tim1_dmaup.Init.Direction = DMA_MEMORY_TO_PERIPH;
		tim1_dmaup.Init.PeriphInc = DMA_PINC_DISABLE;
		tim1_dmaup.Init.MemInc = DMA_MINC_ENABLE;
		tim1_dmaup.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
		tim1_dmaup.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
		tim1_dmaup.Init.Mode = DMA_NORMAL;
		tim1_dmaup.Init.Priority = DMA_PRIORITY_MEDIUM;
		__HAL_LINKDMA(&tim1, hdma[TIM_DMA_ID_UPDATE], tim1_dmaup);
		HAL_DMA_Init(&tim1_dmaup);
		
		HAL_NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
		
		HAL_NVIC_SetPriority(TIM1_TRG_COM_TIM11_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(TIM1_TRG_COM_TIM11_IRQn);
		
		HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM1)
	{
		if(htim->hdma[TIM_DMA_ID_UPDATE]->State == HAL_DMA_STATE_READY)
		{
			u1_printf("定时器1DMA完成中断\r\n");
			HAL_DMA_DeInit(htim->hdma[TIM_DMA_ID_UPDATE]);
		}
		else
		{
			u1_printf("定时器1更新事件\r\n");
		}
	}
}

void HAL_TIM_TriggerCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM1)
	{
		u1_printf("定时器1外部触发事件，次数：%d\r\n", __HAL_TIM_GET_COUNTER(htim));
	}
}

