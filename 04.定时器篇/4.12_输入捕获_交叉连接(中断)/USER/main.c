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
	Timer1_Init(7200, 65536, 0); // 0.1ms记一个数
	
	while (1)
	{
		
	}
}
