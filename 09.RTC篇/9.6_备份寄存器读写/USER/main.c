#include "stm32f1xx_hal.h"

#include "rcc.h"
#include "led.h"
#include "key.h"
#include "uart.h"
#include "timer.h"
#include "i2c.h"
#include "spi.h"
#include "crc.h"
#include "rtc.h"

#include "w25q64.h"

int main(void)
{
	HAL_Init();
	RccClock_Init();
	LED_Init();
	UART1_Init(115200);
	RTC_Init();
	
	u1_printf("字符测试\r\n");
	
	HAL_RTCEx_BKUPWrite(&rtc, 1, 0x1111);
	
	u1_printf("读取的值为： 0x%x\r\n", HAL_RTCEx_BKUPRead(&rtc, 1));
	
	while (1)
	{
		
	}
}

