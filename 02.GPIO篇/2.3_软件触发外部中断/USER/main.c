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
	KEY_Init_IT();

	while (1)
	{
		__HAL_GPIO_EXTI_GENERATE_SWIT(KEY2_PIN);
		HAL_Delay(1000);
	}
}
