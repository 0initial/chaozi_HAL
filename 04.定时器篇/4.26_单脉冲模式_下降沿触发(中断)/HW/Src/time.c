#include "time.h"
#include "uart.h"

TIM_HandleTypeDef              tim1;
TIM_OnePulse_InitTypeDef       tim1_op;

void Timer1_Init(uint16_t psc, uint16_t arr, uint8_t rep)
{
	tim1.Instance = TIM1;
	tim1.Init.Prescaler = psc - 1;
	tim1.Init.CounterMode = TIM_COUNTERMODE_UP;
	tim1.Init.Period = arr - 1;
	tim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV2;
	tim1.Init.RepetitionCounter = rep;
	tim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	HAL_TIM_OnePulse_Init(&tim1, TIM_OPMODE_SINGLE);
	__HAL_TIM_CLEAR_FLAG(&tim1, TIM_FLAG_UPDATE);
	
	tim1_op.OCMode = TIM_OCMODE_PWM1;
	tim1_op.Pulse = 1500;
	tim1_op.OCPolarity = TIM_OCPOLARITY_HIGH;
	tim1_op.ICPolarity = TIM_ICPOLARITY_RISING;
	tim1_op.ICSelection = TIM_ICSELECTION_DIRECTTI;
	tim1_op.ICFilter = 0x8;
	HAL_TIM_OnePulse_ConfigChannel(&tim1, &tim1_op, TIM_CHANNEL_1, TIM_CHANNEL_2);
	HAL_TIM_OnePulse_Start_IT(&tim1, TIM_CHANNEL_1);
}

void HAL_TIM_OnePulse_MspInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef GPIO_InitTypeStruct;
	
	if(htim->Instance == TIM1)
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_TIM1_CLK_ENABLE();
		
		// 定时器1通道1正通道
		GPIO_InitTypeStruct.Pin = TIM1_CH1_PIN;
		GPIO_InitTypeStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitTypeStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(TIM1_CH1_PORT, &GPIO_InitTypeStruct);
		
		// 定时器1通道2正通道
		GPIO_InitTypeStruct.Pin = TIM1_CH2_PIN;
		GPIO_InitTypeStruct.Mode = GPIO_MODE_AF_INPUT;
		GPIO_InitTypeStruct.Pull = GPIO_PULLDOWN;
		HAL_GPIO_Init(TIM1_CH2_PORT, &GPIO_InitTypeStruct);
		
		HAL_NVIC_SetPriority(TIM1_CC_IRQn, 1, 0); // 配置两个C中断，一个输入捕获，一个输出比较
		HAL_NVIC_EnableIRQ(TIM1_CC_IRQn);         // 开启中断
	}
}

// 输出比较中断回调函数
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM1)
	{
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
		{
			u1_printf("定时器1通道1匹配\r\n");
		}
	}
}

// 输入捕获中断回调函数
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM1)
	{
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
		{
			u1_printf("定时器1通道2捕获\r\n");
		}
	}
}

