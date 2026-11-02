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
#include "wdg.h"

#include "w25q64.h"

int main(void)
{
	HAL_Init();
	RccClock_Init();
	KEY_Init_IT();
	UART1_Init(115200);
	IWDG_Init();
	
	u1_printf("×Ö·û²âÊÔ\r\n");
	
	while (1)
	{
		u1_printf("5\r\n");
		HAL_Delay(1000);
		u1_printf("4\r\n");
		HAL_Delay(1000);
		u1_printf("3\r\n");
		HAL_Delay(1000);
		u1_printf("2\r\n");
		HAL_Delay(1000);
		u1_printf("1\r\n");
		HAL_Delay(1000);
	}
}

