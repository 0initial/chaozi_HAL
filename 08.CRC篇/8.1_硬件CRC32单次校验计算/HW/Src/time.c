#include "time.h"
#include "uart.h"

TIM_HandleTypeDef             tim1;
TIM_OC_InitTypeDef            tim1_oc1_pwm;
TIM_OC_InitTypeDef            tim1_oc4_pwm;
TIM_ClearInputConfigTypeDef   tim1_etr_clear;

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
	
	tim1_oc1_pwm.OCMode = TIM_OCMODE_PWM1;
	tim1_oc1_pwm.Pulse = 999;
	tim1_oc1_pwm.OCPolarity = TIM_OCPOLARITY_LOW;
	tim1_oc1_pwm.OCFastMode = TIM_OCFAST_DISABLE;
	HAL_TIM_PWM_ConfigChannel(&tim1, &tim1_oc1_pwm, TIM_CHANNEL_1);
	
	tim1_oc4_pwm.OCMode = TIM_OCMODE_PWM1;
	tim1_oc4_pwm.Pulse = 999;
	tim1_oc4_pwm.OCPolarity = TIM_OCPOLARITY_LOW;
	tim1_oc4_pwm.OCFastMode = TIM_OCFAST_DISABLE;
	HAL_TIM_PWM_ConfigChannel(&tim1, &tim1_oc4_pwm, TIM_CHANNEL_4);
	
	// 配置 TIM1 的 OCRef 清除功能（通过 ETR 引脚快速关断输出）
	tim1_etr_clear.ClearInputState = ENABLE;                                 // 启用 OCRef 清除功能：当条件满足时，强制清零通道输出
	tim1_etr_clear.ClearInputSource = TIM_CLEARINPUTSOURCE_ETR;              // 清除信号来自外部触发输入引脚（ETR）
	tim1_etr_clear.ClearInputPolarity = TIM_CLEARINPUTPOLARITY_NONINVERTED;  // ETR 高电平有效（即 ETR=1 时触发清除）
	tim1_etr_clear.ClearInputPrescaler = 0;                                  // ETR 预分频器关闭（必须为 0，符合 OCRef Clear 要求）
	tim1_etr_clear.ClearInputFilter = 0x8;                                   // 数字滤波器设为 8 级采样，抑制噪声误触发
	HAL_TIM_ConfigOCrefClear(&tim1, &tim1_etr_clear, TIM_CHANNEL_4);         // 将上述清除配置应用到 TIM1 的通道 4
	
	HAL_TIM_PWM_Start_IT(&tim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start_IT(&tim1, TIM_CHANNEL_4);
}

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
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

		// 定时器1通道4正通道
		GPIO_InitTypeStruct.Pin = TIM1_CH4_PIN;
		GPIO_InitTypeStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitTypeStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(TIM1_CH4_PORT, &GPIO_InitTypeStruct);
		
		// 定时器1 ETR 通道
		GPIO_InitTypeStruct.Pin = TIM1_ETR_PIN;
		GPIO_InitTypeStruct.Mode = GPIO_MODE_AF_INPUT;
		GPIO_InitTypeStruct.Pull = GPIO_PULLDOWN;
		HAL_GPIO_Init(TIM1_ETR_PORT, &GPIO_InitTypeStruct);
		
		HAL_NVIC_SetPriority(TIM1_CC_IRQn, 1, 0); // 配置两个C中断，一个输入捕获，一个输出比较
		HAL_NVIC_EnableIRQ(TIM1_CC_IRQn);         // 开启中断
	}
}

