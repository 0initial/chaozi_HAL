#include "stm32f1xx_hal.h"
#include "rcc.h"
#include "led.h"
#include "key.h"

int main(void)
{
	HAL_Init();
	RccClock_Init();
	LED_Init();
//	KEY_Init();
//	KEY_Init_IT(0);
	KEY_Init_Event();
	
	while (1)
	{
		for(uint8_t i = 0; i < 6; i++)
		{
			HAL_GPIO_TogglePin(LED_RED_PORT, LED_RED_PIN);
			HAL_Delay(500);
		}
		LED_All_DISABLE();
		
		HAL_SuspendTick();
		// 由于有系统滴答时钟，导致不断进入中断，所以不会进入睡眠
		HAL_PWR_EnterSLEEPMode(0, PWR_SLEEPENTRY_WFE);
		
		// 睡眠后，只有唤醒才可以继续从这里开始运行
		HAL_ResumeTick();
	}
}
