#include "stm32f1xx_hal.h"
#include "rcc.h"
#include "led.h"
#include "key.h"
#include "uart.h"
#include "time.h"

int main(void)
{
	HAL_Init();
	RccClock_Init();
	LED_Init();
	KEY_Init_IT(0);
	UART1_Init(115200);
	Timer1_Init(36000, 2000, 1);
	
	while (1)
	{
		if(__HAL_TIM_GET_FLAG(&tim1, TIM_FLAG_UPDATE))
		{
			__HAL_TIM_CLEAR_FLAG(&tim1, TIM_FLAG_UPDATE);
			u1_printf("定时器1定时时间到\r\n");
		}
	}
}
