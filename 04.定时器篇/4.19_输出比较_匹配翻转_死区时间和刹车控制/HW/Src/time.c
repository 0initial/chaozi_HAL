#include "time.h"
#include "uart.h"

TIM_HandleTypeDef        tim1;
TIM_OC_InitTypeDef       tim1_oc1;
DMA_HandleTypeDef        tim1_oc1_dma;

TIM_BreakDeadTimeConfigTypeDef tim1_db;

uint32_t count = 0;

void Timer1_Init(uint16_t psc, uint16_t arr, uint8_t rep)
{
	tim1.Instance = TIM1;
	tim1.Init.Prescaler = psc - 1;
	tim1.Init.CounterMode = TIM_COUNTERMODE_CENTERALIGNED3;
	tim1.Init.Period = arr - 1;
	tim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV2;
	tim1.Init.RepetitionCounter = rep;
	tim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	HAL_TIM_OC_Init(&tim1);
	__HAL_TIM_CLEAR_FLAG(&tim1, TIM_FLAG_UPDATE);
	
	tim1_oc1.OCMode = TIM_OCMODE_TOGGLE;
	tim1_oc1.Pulse = 100 - 1;
	tim1_oc1.OCPolarity = TIM_OCPOLARITY_HIGH;
	// 是当前选择的相反通道，选择高，那么互补输出低，而不是互补输出高
	tim1_oc1.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	tim1_oc1.OCIdleState = TIM_OCIDLESTATE_RESET;
	tim1_oc1.OCNIdleState = TIM_OCNIDLESTATE_SET;
	HAL_TIM_OC_ConfigChannel(&tim1, &tim1_oc1, TIM_CHANNEL_1);
	
	tim1_db.OffStateRunMode = TIM_OSSR_DISABLE;            // 运行模式下，关闭状态不生效（输出正常）
	tim1_db.OffStateIDLEMode = TIM_OSSI_DISABLE;           // 空闲模式下，关闭状态不生效（输出保持）
	tim1_db.LockLevel = TIM_LOCKLEVEL_OFF;                 // 不锁定寄存器，允许随时修改配置
	tim1_db.DeadTime = 0xFF;                               // 死区时间设为最大值附近（实际值需根据时钟计算，此处示例） 28us
	tim1_db.BreakState = TIM_BREAK_ENABLE;                 // 启用刹车（Break）功能
	tim1_db.BreakPolarity = TIM_BREAKPOLARITY_HIGH;        // 刹车信号高电平有效
	tim1_db.BreakFilter = 0x8;                             // 刹车输入滤波器设为 8 个采样周期（抗干扰）
	tim1_db.AutomaticOutput = TIM_AUTOMATICOUTPUT_ENABLE;  // 允许自动恢复 PWM 输出（配合主输出使能）
	HAL_TIMEx_ConfigBreakDeadTime(&tim1, &tim1_db);
	
	HAL_TIM_OC_Start_IT(&tim1, TIM_CHANNEL_1);
	HAL_TIMEx_OCN_Start_IT(&tim1, TIM_CHANNEL_1);
}

void HAL_TIM_OC_MspInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	if(htim->Instance == TIM1)
	{
		__HAL_RCC_TIM1_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_GPIOB_CLK_ENABLE();
		__HAL_RCC_DMA1_CLK_ENABLE();
		
		// 定时器1的通道1 PA8
		GPIO_InitStruct.Pin = TIM1_CH1_PIN;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
		HAL_GPIO_Init(TIM1_CH1_PORT, &GPIO_InitStruct);
		
		// 定时器1的刹车通道
		GPIO_InitStruct.Pin = TIM1_BKIN_PIN;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_INPUT;
		GPIO_InitStruct.Pull = GPIO_PULLDOWN;
		HAL_GPIO_Init(TIM1_BKIN_PORT, &GPIO_InitStruct);
		
		// 定时器1的通道1互补通道 PB13
		GPIO_InitStruct.Pin = TIM1_CH1N_PIN;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
		HAL_GPIO_Init(TIM1_CH1N_PORT, &GPIO_InitStruct);
		
		HAL_NVIC_SetPriority(TIM1_BRK_TIM9_IRQn, 1, 0);
		HAL_NVIC_EnableIRQ(TIM1_BRK_TIM9_IRQn);
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
			
		}
	}
}

// 定时器刹车中断回调函数
void HAL_TIMEx_BreakCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM1)
	{
		count++;
	}
}

