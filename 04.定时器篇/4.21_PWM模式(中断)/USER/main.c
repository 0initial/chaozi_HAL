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
	Timer1_Init(36000, 1000, 0); // 0.5ms记一个数
	
	while (1)
	{
		
	}
}
