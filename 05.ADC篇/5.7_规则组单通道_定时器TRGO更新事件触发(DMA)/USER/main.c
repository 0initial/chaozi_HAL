#include "stm32f1xx_hal.h"
#include "rcc.h"
#include "led.h"
#include "key.h"
#include "uart.h"
#include "time.h"
#include "adc.h"

int main(void)
{
	HAL_Init();
	RccClock_Init();
	LED_Init();
	UART1_Init(115200);
	ADC_Init();
	Timer3_Init(7200, 100); // 0.1ms一个上升沿，100则10ms
	
	while (1)
	{
		
	}
}
