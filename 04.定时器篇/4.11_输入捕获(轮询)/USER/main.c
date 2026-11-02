#include "stm32f1xx_hal.h"
#include "rcc.h"
#include "led.h"
#include "key.h"
#include "uart.h"
#include "time.h"

uint16_t count = 0;

int main(void)
{
	HAL_Init();
	RccClock_Init();
	LED_Init();
	KEY_Init_IT();
	UART1_Init(115200);
	Timer1_Init(7200, 65536, 0); // 0.1ms记一个数
	
	while (1)
	{
		if(__HAL_TIM_GET_FLAG(&tim1, TIM_FLAG_UPDATE))
		{
			__HAL_TIM_CLEAR_FLAG(&tim1, TIM_FLAG_UPDATE);
			u1_printf("定时器1更新事件，count：%d\r\n", ++count);
		}
		if(__HAL_TIM_GET_FLAG(&tim1, TIM_FLAG_CC1))
		{
			__HAL_TIM_CLEAR_FLAG(&tim1, TIM_FLAG_CC1);
			u1_printf("定时器1通道1捕获：%d\r\n", HAL_TIM_ReadCapturedValue(&tim1, TIM_CHANNEL_1) + (count * 65536));
		}
		if(__HAL_TIM_GET_FLAG(&tim1, TIM_FLAG_CC2))
		{
			__HAL_TIM_CLEAR_FLAG(&tim1, TIM_FLAG_CC2);
			u1_printf("定时器1通道2捕获：%d\r\n", HAL_TIM_ReadCapturedValue(&tim1, TIM_CHANNEL_2) + (count * 65536));
		}
	}
}
