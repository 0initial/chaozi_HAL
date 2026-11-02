#include "time.h"
#include "uart.h"

TIM_HandleTypeDef        tim1;
TIM_OC_InitTypeDef       tim1_oc1;
DMA_HandleTypeDef        tim1_oc1_dma;

uint8_t count = 0;
uint16_t tim1_oc1_dmabuff[] = {200 - 1, 300 - 1, 400 - 1, 500 - 1, 600 - 1, 700 - 1, 800 - 1, 900 - 1, 1000 - 1, 100 - 1};

void Timer1_Init(uint16_t psc, uint16_t arr, uint8_t rep)
{
	tim1.Instance = TIM1;
	tim1.Init.Prescaler = psc - 1;
	tim1.Init.CounterMode = TIM_COUNTERMODE_CENTERALIGNED1;
	tim1.Init.Period = arr - 1;
	tim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	tim1.Init.RepetitionCounter = rep;
	tim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	HAL_TIM_OC_Init(&tim1);
	__HAL_TIM_CLEAR_FLAG(&tim1, TIM_FLAG_UPDATE);
	
	tim1_oc1.OCMode = TIM_OCMODE_TOGGLE;
	tim1_oc1.Pulse = 100 - 1;
	tim1_oc1.OCPolarity = TIM_OCPOLARITY_HIGH;
	HAL_TIM_OC_ConfigChannel(&tim1, &tim1_oc1, TIM_CHANNEL_1);
	
	HAL_TIM_OC_Start_DMA(&tim1, TIM_CHANNEL_1, (uint32_t *)tim1_oc1_dmabuff, 10);
}

void HAL_TIM_OC_MspInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	if(htim->Instance == TIM1)
	{
		__HAL_RCC_TIM1_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_DMA1_CLK_ENABLE();
		
		// 定时器1的通道1 PA8
		GPIO_InitStruct.Pin = TIM1_CH1_PIN;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
		HAL_GPIO_Init(TIM1_CH1_PORT, &GPIO_InitStruct);
		
		tim1_oc1_dma.Instance = DMA1_Channel2;
		tim1_oc1_dma.Init.Direction = DMA_MEMORY_TO_PERIPH;
		tim1_oc1_dma.Init.PeriphInc = DMA_PINC_DISABLE;
		tim1_oc1_dma.Init.MemInc = DMA_MINC_ENABLE;
		tim1_oc1_dma.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
		tim1_oc1_dma.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
		tim1_oc1_dma.Init.Mode = DMA_CIRCULAR;
		tim1_oc1_dma.Init.Priority = DMA_PRIORITY_MEDIUM;
		__HAL_LINKDMA(&tim1, hdma[TIM_DMA_ID_CC1], tim1_oc1_dma);
		HAL_DMA_Init(&tim1_oc1_dma);
		
		HAL_NVIC_SetPriority(DMA1_Channel2_IRQn, 1, 0);
		HAL_NVIC_EnableIRQ(DMA1_Channel2_IRQn);
	}
}

// 定时器更新中断回调函数
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM1)
	{
		
	}
}

// 定时器输出比较中断回调函数
void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM1)
	{
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
		{
			u1_printf("定时器1通道1匹配，次数：%d\r\n", ++count);
			if(count >= 5)
			{
				HAL_TIM_OC_Stop_IT(&tim1, TIM_CHANNEL_1);
			}
		}
	}
}

// DMA半完成
void HAL_TIM_PWM_PulseFinishedHalfCpltCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM1)
	{
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
		{
			u1_printf("定时器1通道1 DMA半完成\r\n");
		}
	}
}

// DMA完成
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM1)
	{
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
		{
			u1_printf("定时器1通道1 DMA完成\r\n");
		}
	}
}

