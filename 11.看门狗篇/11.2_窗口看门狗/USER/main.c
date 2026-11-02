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
	WWDG_Init();
	
	u1_printf("字符测试\r\n");
	
	while (1)
	{
		HAL_Delay(500);
		u1_printf("正常运行\r\n");
	}
}

