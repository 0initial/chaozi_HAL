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
	UART1_Init(115200);
	Timer1_Init(36000, 2000, 0); // 1s一个周期
	
	while (1)
	{
		if(__HAL_TIM_GET_FLAG(&tim1, TIM_FLAG_CC1))
		{
			__HAL_TIM_CLEAR_FLAG(&tim1, TIM_FLAG_CC1);
			u1_printf("定时器1通道1匹配\r\n");
		}
		if(__HAL_TIM_GET_FLAG(&tim1, TIM_FLAG_CC2))
		{
			__HAL_TIM_CLEAR_FLAG(&tim1, TIM_FLAG_CC2);
			u1_printf("定时器1通道2捕获\r\n");
		}
	}
}
