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

	while (1)
	{
//		switch(Key_Scan(0))
//		{
//			case 1:
//				HAL_GPIO_TogglePin(LED_RED_PORT, LED_RED_PIN);
//				break;
//			
//			case 2:
//				HAL_GPIO_TogglePin(LED_RED_PORT, LED_RED_PIN);
//				break;
//			
//			default:
//				break;
//		}
	}
}
