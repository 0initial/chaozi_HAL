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
	KEY_Init_IT();
	UART1_Init(115200);
	Timer1_Init(1, 5, 0); // 按键5次产生更新
	
	while (1)
	{
		if(__HAL_TIM_GET_FLAG(&tim1, TIM_FLAG_UPDATE))
		{
			__HAL_TIM_CLEAR_FLAG(&tim1, TIM_FLAG_UPDATE);
			u1_printf("定时器1更新事件\r\n");
		}
		if(__HAL_TIM_GET_FLAG(&tim1, TIM_FLAG_TRIGGER))
		{
			__HAL_TIM_CLEAR_FLAG(&tim1, TIM_FLAG_TRIGGER);
			u1_printf("定时器1外部触发事件，次数：%d\r\n", __HAL_TIM_GET_COUNTER(&tim1));
			if(__HAL_TIM_GET_COUNTER(&tim1) == 3)
			{
				__HAL_TIM_SET_AUTORELOAD(&tim1, 9);
				u1_printf("ARR重装载值更改为10\r\n");
			}
		}
	}
}
