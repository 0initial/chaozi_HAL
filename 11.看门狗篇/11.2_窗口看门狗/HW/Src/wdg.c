#include "wdg.h"
#include "uart.h"

IWDG_HandleTypeDef iwdg; // 独立看门狗总控
WWDG_HandleTypeDef wwdg; // 窗口看门狗总控

// 独立看门狗初始化
void IWDG_Init(void)
{
	iwdg.Instance = IWDG;                    // 使用独立看门狗
	iwdg.Init.Prescaler = IWDG_PRESCALER_32; // 32分频
	iwdg.Init.Reload = 0xFFF;                // 计数值0xFFF 喂狗超时时间 3276.8ms
	HAL_IWDG_Init(&iwdg);                    // 初始化独立看门狗
}

// 窗口看门狗初始化
void WWDG_Init(void)
{
	wwdg.Instance = WWDG;
	wwdg.Init.Prescaler = WWDG_PRESCALER_8;
	wwdg.Init.Counter = 0x7F;
	wwdg.Init.Window = 0x5F;
	wwdg.Init.EWIMode = WWDG_EWI_ENABLE;
	HAL_WWDG_Init(&wwdg);
}

void HAL_WWDG_MspInit(WWDG_HandleTypeDef *hwwdg)
{
	__HAL_RCC_WWDG_CLK_ENABLE();
	
	HAL_NVIC_SetPriority(WWDG_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ(WWDG_IRQn);
}

// 窗口看门狗中断回调
void HAL_WWDG_EarlyWakeupCallback(WWDG_HandleTypeDef *hwwdg)
{
	HAL_WWDG_Refresh(&wwdg);
}



