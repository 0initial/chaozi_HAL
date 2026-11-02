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
	KEY_Init();
	UART1_Init(115200);
	RTC_Init();
	
	u1_printf("字符测试\r\n");
	
	for(uint8_t i = 1; i <= 10; i++)
	{
		u1_printf("编号%d的备份寄存器读取的值为： 0x%x\r\n", i, HAL_RTCEx_BKUPRead(&rtc, i));
	}
	
	while (1)
	{
		switch(Key_Scan(0))
		{
			case 1:
				HAL_RTCEx_BKUPWrite(&rtc, 1, 0x1111);
				HAL_RTCEx_BKUPWrite(&rtc, 2, 0x2222);
				HAL_RTCEx_BKUPWrite(&rtc, 3, 0x3333);
				HAL_RTCEx_BKUPWrite(&rtc, 4, 0x4444);
				HAL_RTCEx_BKUPWrite(&rtc, 5, 0x5555);
				HAL_RTCEx_BKUPWrite(&rtc, 6, 0x6666);
				HAL_RTCEx_BKUPWrite(&rtc, 7, 0x7777);
				HAL_RTCEx_BKUPWrite(&rtc, 8, 0x8888);
				HAL_RTCEx_BKUPWrite(&rtc, 9, 0x9999);
				HAL_RTCEx_BKUPWrite(&rtc, 10, 0xAAAA);
				u1_printf("备份寄存器写入完成\r\n");
				break;
			
			case 2:
				break;
		}
	}
}

