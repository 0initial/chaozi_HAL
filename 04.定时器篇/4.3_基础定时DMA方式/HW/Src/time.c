#include "time.h"
#include "uart.h"

TIM_HandleTypeDef tim1;

DMA_HandleTypeDef tim1_dmaup;

uint16_t tim1_dmabuff[4] = {3999, 5999, 7999, 9999};

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
	__HAL_TIM_ENABLE_IT(&tim1, TIM_IT_UPDATE);
	HAL_TIM_Base_Start_DMA(&tim1, (uint32_t *)tim1_dmabuff, 4);
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM1)
	{
		__HAL_RCC_TIM1_CLK_ENABLE();
		__HAL_RCC_DMA1_CLK_ENABLE();

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
		
		// 开启中断
		HAL_NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
		
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
			u1_printf("定时器1DMA中断\r\n");
			HAL_DMA_DeInit(&tim1_dmaup); 
		}
		else
		{
			u1_printf("定时器1中断\r\n");
		}
	}
}

