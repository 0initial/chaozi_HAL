#include "time.h"
#include "uart.h"

TIM_HandleTypeDef              tim1;
TIM_OC_InitTypeDef             tim1_oc1_pwm;
DMA_HandleTypeDef              tim1_oc1_pwmdma;
TIM_BreakDeadTimeConfigTypeDef tim1_db;

uint16_t tim1_oc1_pwmdmabuff[4] = {600 - 1, 800 - 1, 1000 - 1, 1200 - 1};

uint32_t count = 0;

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
	tim1_oc1_pwm.Pulse = 400;
	// 有效电平
	tim1_oc1_pwm.OCPolarity = TIM_OCPOLARITY_HIGH;
	tim1_oc1_pwm.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	// 空闲电平
	tim1_oc1_pwm.OCIdleState = TIM_OCIDLESTATE_RESET;
	tim1_oc1_pwm.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	tim1_oc1_pwm.OCFastMode = TIM_OCFAST_DISABLE;
	HAL_TIM_PWM_ConfigChannel(&tim1, &tim1_oc1_pwm, TIM_CHANNEL_1);
	
	// 定时器1死区时间和刹车配置
	tim1_db.OffStateRunMode = TIM_OSSR_DISABLE;
	tim1_db.OffStateIDLEMode = TIM_OSSI_DISABLE;
	tim1_db.LockLevel = TIM_LOCKLEVEL_OFF;
	tim1_db.DeadTime = 0xFF;
	tim1_db.BreakState = TIM_BREAK_ENABLE;
	tim1_db.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
	tim1_db.BreakFilter = 0x8;
	tim1_db.AutomaticOutput = TIM_AUTOMATICOUTPUT_ENABLE;
	HAL_TIMEx_ConfigBreakDeadTime(&tim1, &tim1_db);
	
	// 正通道开启PWM通道
	HAL_TIM_PWM_Start_DMA(&tim1, TIM_CHANNEL_1, (uint32_t *)tim1_oc1_pwmdmabuff, 4);
	// 互补通道开启PWM通道 正通道开启函数已经调用DMA，互补通道则不需要了
	HAL_TIMEx_PWMN_Start(&tim1, TIM_CHANNEL_1);
	// 手动开启定时器1的刹车中断
	__HAL_TIM_ENABLE_IT(&tim1, TIM_IT_BREAK);
}

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef GPIO_InitTypeStruct;
	
	if(htim->Instance == TIM1)
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_GPIOB_CLK_ENABLE();
		__HAL_RCC_TIM1_CLK_ENABLE();
		__HAL_RCC_DMA1_CLK_ENABLE();
		
		// 定时器1通道1正通道
		GPIO_InitTypeStruct.Pin = TIM1_CH1_PIN;
		GPIO_InitTypeStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitTypeStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(TIM1_CH1_PORT, &GPIO_InitTypeStruct);
		
		// 定时器1通道1互补通道
		GPIO_InitTypeStruct.Pin = TIM1_CH1N_PIN;
		GPIO_InitTypeStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitTypeStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(TIM1_CH1N_PORT, &GPIO_InitTypeStruct);
		
		// 定时器1刹车通道
		GPIO_InitTypeStruct.Pin = TIM1_BKIN_PIN;
		GPIO_InitTypeStruct.Mode = GPIO_MODE_AF_INPUT;
		GPIO_InitTypeStruct.Pull = GPIO_PULLDOWN;
		HAL_GPIO_Init(TIM1_BKIN_PORT, &GPIO_InitTypeStruct);
		
		// 定时器1通道1DMA配置
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
		
		HAL_NVIC_SetPriority(TIM1_BRK_TIM9_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(TIM1_BRK_TIM9_IRQn);
	}
}

// 输出比较DMA传输中断回调函数
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

// 定时器刹车中断回调函数
void HAL_TIMEx_BreakCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM1)
	{
		// 刹车是电平判断而不是电沿判断，如果一直刹车，会一直进入中断，所以刹车中断回调要注意调用
		count++;
	}
}



