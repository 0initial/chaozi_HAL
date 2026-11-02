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
	
	HAL_ADC_PollForConversion(&adc1, 1000);
	u1_printf("ADC1_IN0: %d \r\n", HAL_ADC_GetValue(&adc1));
	
	while (1)
	{
		
	}
}
