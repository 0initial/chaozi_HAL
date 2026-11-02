#include "wdg.h"
#include "uart.h"

IWDG_HandleTypeDef iwdg; // 独立看门狗总控

void IWDG_Init(void)
{
	iwdg.Instance = IWDG;                    // 使用独立看门狗
	iwdg.Init.Prescaler = IWDG_PRESCALER_32; // 32分频
	iwdg.Init.Reload = 0xFFF;                // 计数值0xFFF 喂狗超时时间 3276.8ms
	HAL_IWDG_Init(&iwdg);                    // 初始化独立看门狗
}


