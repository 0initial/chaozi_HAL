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
//	LED_Init();
	UART1_Init(115200);
	KEY_Init_IT();
	Timer3_Init(36000, 0xFFFF); // 0.1ms一个上升沿，100则10ms
	ADC_Init();
	
	while (1)
	{
		
	}
}
