#include "time.h"
#include "uart.h"

TIM_HandleTypeDef        tim1;
TIM_OC_InitTypeDef       tim1_oc1_pwm;

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
	
	// PWMÅäÖÃ
	tim1_oc1_pwm.OCMode = TIM_OCMODE_PWM1;
	tim1_oc1_pwm.Pulse = 200;
	tim1_oc1_pwm.OCPolarity = TIM_OCPOLARITY_LOW;
	tim1_oc1_pwm.OCFastMode = TIM_OCFAST_DISABLE;
	HAL_TIM_PWM_ConfigChannel(&tim1, &tim1_oc1_pwm, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&tim1, TIM_CHANNEL_1);
}

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef GPIO_InitTypeStruct;
	
	if(htim->Instance == TIM1)
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_TIM1_CLK_ENABLE();
		
		GPIO_InitTypeStruct.Pin = TIM1_CH1_PIN;
		GPIO_InitTypeStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitTypeStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(TIM1_CH1_PORT, &GPIO_InitTypeStruct);
	}
}
