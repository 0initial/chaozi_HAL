#include "time.h"
#include "uart.h"

TIM_HandleTypeDef        tim1;
TIM_OC_InitTypeDef       tim1_oc1_pwm;
DMA_HandleTypeDef        tim1_oc1_pwmdma;

uint16_t tim1_oc1_pwmdmabuff[4] = {1200 - 1, 1400 - 1, 1600 - 1, 1800 - 1};

void Timer1_Init(uint16_t psc, uint16_t arr, uint8_t rep)
{
	tim1.Instance = TIM1;
	tim1.Init.Prescaler = psc - 1;
	tim1.Init.CounterMode = TIM_COUNTERMODE_UP;
	tim1.Init.Period = arr - 1;
	tim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV2;
	tim1.Init.RepetitionCounter = rep;
	tim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	HAL_TIM_PWM_Init(&tim1);
	__HAL_TIM_CLEAR_FLAG(&tim1, TIM_FLAG_UPDATE);
	
	// PWM配置
	tim1_oc1_pwm.OCMode = TIM_OCMODE_PWM1;
	tim1_oc1_pwm.Pulse = 1000;
	tim1_oc1_pwm.OCPolarity = TIM_OCPOLARITY_LOW;
	tim1_oc1_pwm.OCFastMode = TIM_OCFAST_DISABLE;
	HAL_TIM_PWM_ConfigChannel(&tim1, &tim1_oc1_pwm, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start_DMA(&tim1, TIM_CHANNEL_1, (uint32_t *)tim1_oc1_pwmdmabuff, 4);
}

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef GPIO_InitTypeStruct;
	
	if(htim->Instance == TIM1)
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_TIM1_CLK_ENABLE();
		__HAL_RCC_DMA1_CLK_ENABLE();
		
		GPIO_InitTypeStruct.Pin = TIM1_CH1_PIN;
		GPIO_InitTypeStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitTypeStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(TIM1_CH1_PORT, &GPIO_InitTypeStruct);
		
		tim1_oc1_pwmdma.Instance = DMA1_Channel2;
		tim1_oc1_pwmdma.Init.Direction = DMA_MEMORY_TO_PERIPH;
		tim1_oc1_pwmdma.Init.PeriphInc = DMA_PINC_DISABLE;
		tim1_oc1_pwmdma.Init.MemInc = DMA_MINC_ENABLE;
		tim1_oc1_pwmdma.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
		tim1_oc1_pwmdma.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
		tim1_oc1_pwmdma.Init.Mode = DMA_CIRCULAR;
		tim1_oc1_pwmdma.Init.Priority = DMA_PRIORITY_MEDIUM;
		__HAL_LINKDMA(htim, hdma[TIM_DMA_ID_CC1], tim1_oc1_pwmdma);
		HAL_DMA_Init(&tim1_oc1_pwmdma);
		
		
		HAL_NVIC_SetPriority(DMA1_Channel2_IRQn, 1, 0);
		HAL_NVIC_EnableIRQ(DMA1_Channel2_IRQn);
	}
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM1)
	{
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
		{
			u1_printf("定时器1通道1完成中断\r\n");
		}
	}
}



