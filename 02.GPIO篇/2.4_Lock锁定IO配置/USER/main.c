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
	KEY_Init_IT(0);

	HAL_Delay(5000);
	HAL_GPIO_LockPin(KEY2_PROT, KEY2_PIN);
	HAL_GPIO_DeInit(KEY2_PROT, KEY2_PIN);
	while (1)
	{
		
	}
}
