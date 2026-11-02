#include "stm32f1xx_hal.h"
#include "time.h"

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

uint8_t sec = 0xFF;
struct tm *GetTime;

int main(void)
{
	HAL_Init();
	RccClock_Init();
	UART1_Init(115200);
	RTC_Init();
	
	u1_printf("×Ö·û²âÊÔ\r\n");
	
	while (1)
	{
		GetTime = GetTimeData();
		if(sec != GetTime->tm_sec)
		{
			sec = GetTime->tm_sec;
			u1_printf("%d-%d-%d %d:%d;%d\r\n", GetTime->tm_year + 1900, GetTime->tm_mon, GetTime->tm_mday, GetTime->tm_hour, GetTime->tm_min, GetTime->tm_sec);
		}
	}
}

