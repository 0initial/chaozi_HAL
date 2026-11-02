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
	
	uint8_t res, count;
	
	count = 0;
	
	while (1)
	{
		res = HAL_ADC_PollForConversion(&adc1, 1000);
		if(res == HAL_OK)
		{
			u1_printf("ADC1_IN11 数字量: %d \r\n", HAL_ADC_GetValue(&adc1));
			u1_printf("ADC1_IN11 模拟量: %f \r\n", HAL_ADC_GetValue(&adc1) * (3.3 / 4096));
			u1_printf("次数: %d \r\n", ++count);
		}
		else if(res == HAL_TIMEOUT)
		{
			u1_printf("ADC1_IN0:转换超时\r\n");
		}
		
		if(count >= 5)
		{
			HAL_ADC_Stop(&adc1);
			u1_printf("次数 = %d,停止ADC转换 \r\n", count);
			count = 0;
		}
		
		HAL_Delay(1000);
	}
}
