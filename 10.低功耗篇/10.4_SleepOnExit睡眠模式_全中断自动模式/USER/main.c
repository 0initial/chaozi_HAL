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
	KEY_Init_IT();
	UART1_Init(115200);
	
	HAL_PWR_EnableSleepOnExit();
	Timer1_Init(36000, 10000, 0);
	
	u1_printf("×Ö·û²âÊÔ\r\n");
	
	while (1)
	{
		
	}
}

