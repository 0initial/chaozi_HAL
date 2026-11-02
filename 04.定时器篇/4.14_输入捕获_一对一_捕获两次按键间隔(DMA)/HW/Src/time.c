#include "time.h"
#include "uart.h"

TIM_HandleTypeDef        tim1;
DMA_HandleTypeDef        tim1_dmaup;
TIM_ClockConfigTypeDef   tim1_clock;
TIM_IC_InitTypeDef       tim1_ic1;
TIM_IC_InitTypeDef       tim1_ic2;

DMA_HandleTypeDef        tim1_ic1_dma;
DMA_HandleTypeDef        tim1_ic2_dma;

uint32_t key1_set1, key1_set2, key2_set1, key2_set2;
uint16_t count = 0;
uint16_t tim1_ic1_dmabuff[2];
uint16_t tim1_ic2_dmabuff[2];

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
	tim1_ic1.ICSelection = TIM_ICSELECTION_DIRECTTI;
	tim1_ic1.ICPrescaler = TIM_ICPSC_DIV1;
	tim1_ic1.ICFilter = 0x08;
	HAL_TIM_IC_ConfigChannel(&tim1, &tim1_ic1, TIM_CHANNEL_1);
	
	tim1_ic2.ICPolarity = TIM_ICPOLARITY_RISING;
	tim1_ic2.ICSelection = TIM_ICSELECTION_DIRECTTI;
	tim1_ic2.ICPrescaler = TIM_ICPSC_DIV1;
	tim1_ic2.ICFilter = 0x08;
	HAL_TIM_IC_ConfigChannel(&tim1, &tim1_ic2, TIM_CHANNEL_2);
	
	__HAL_TIM_ENABLE_IT(&tim1, TIM_IT_UPDATE);
	HAL_TIM_IC_Start_DMA(&tim1, TIM_CHANNEL_1, (uint32_t *)tim1_ic1_dmabuff, 2);
	// DMA内部初始化，会让DMA变成BUSY状态，影响下一次的DMA初始化，所以这里手动更改状态
	tim1.State = HAL_TIM_STATE_READY;
	HAL_TIM_IC_Start_DMA(&tim1, TIM_CHANNEL_2, (uint32_t *)tim1_ic2_dmabuff, 2);
}

void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	if(htim->Instance == TIM1)
	{
		__HAL_RCC_TIM1_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_DMA1_CLK_ENABLE();
		
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
		
		tim1_ic1_dma.Instance = DMA1_Channel2;
		tim1_ic1_dma.Init.Direction = DMA_PERIPH_TO_MEMORY;
		tim1_ic1_dma.Init.PeriphInc = DMA_PINC_DISABLE;
		tim1_ic1_dma.Init.MemInc = DMA_MINC_ENABLE;
		tim1_ic1_dma.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
		tim1_ic1_dma.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
		tim1_ic1_dma.Init.Mode = DMA_CIRCULAR;
		tim1_ic1_dma.Init.Priority = DMA_PRIORITY_MEDIUM;
		__HAL_LINKDMA(&tim1, hdma[TIM_DMA_ID_CC1], tim1_ic1_dma);
		HAL_DMA_Init(&tim1_ic1_dma);
		
		tim1_ic2_dma.Instance = DMA1_Channel3;
		tim1_ic2_dma.Init.Direction = DMA_PERIPH_TO_MEMORY;
		tim1_ic2_dma.Init.PeriphInc = DMA_PINC_DISABLE;
		tim1_ic2_dma.Init.MemInc = DMA_MINC_ENABLE;
		tim1_ic2_dma.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
		tim1_ic2_dma.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
		tim1_ic2_dma.Init.Mode = DMA_CIRCULAR;
		tim1_ic2_dma.Init.Priority = DMA_PRIORITY_MEDIUM;
		__HAL_LINKDMA(&tim1, hdma[TIM_DMA_ID_CC2], tim1_ic2_dma);
		HAL_DMA_Init(&tim1_ic2_dma);
		
		HAL_NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 1, 0);
		HAL_NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
	
		HAL_NVIC_SetPriority(DMA1_Channel2_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(DMA1_Channel2_IRQn);
		
		HAL_NVIC_SetPriority(DMA1_Channel3_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM1)
	{
		u1_printf("定时器1更新事件，count：%d\r\n", ++count);
	}
}

// DMA半完成中断
void HAL_TIM_IC_CaptureHalfCpltCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM1)
	{
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
		{
			key1_set1 = count;
			u1_printf("按键1第一次按下\r\n");
		}
		else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
		{
			key2_set1 = count;
			u1_printf("按键2第一次按下\r\n");
		}
	}
}

// DMA完成中断
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM1)
	{
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
		{
			key1_set2 = count;
			u1_printf("按键1第二次按下\r\n");
			u1_printf("两次按键按下时间间隔：%d\r\n", ((tim1_ic1_dmabuff[1] + key1_set2 * 65536) - (tim1_ic1_dmabuff[0] + key1_set1 * 65536)) / 10);
		}
		else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
		{
			key2_set2 = count;
			u1_printf("按键2第二次按下\r\n");
			u1_printf("两次按键按下时间间隔：%d\r\n", ((tim1_ic2_dmabuff[1] + key2_set2 * 65536) - (tim1_ic2_dmabuff[0] + key2_set1 * 65536)) / 10);
		}
	}
}

