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
//	KEY_Init_Event();
//	KEY_Init_EventOut();
	
	while (1)
	{
		
	}
}
