#include "stm32f1xx_hal.h"
#include "rcc.h"
#include "led.h"

int main(void)
{
	HAL_Init();
	RccClock_Init();

	LED_Init();

	while (1)
	{
		LED_Red();
		HAL_Delay(500);
		LED_Green();
		HAL_Delay(500);
		LED_Blue();
		HAL_Delay(500);
		LED_Yellow();
		HAL_Delay(500);
		LED_Purple();
		HAL_Delay(500);
		LED_Cyan();
		HAL_Delay(500);
	}
}
