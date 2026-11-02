#include "stm32f1xx_hal.h"
#include "rcc.h"
#include "led.h"
#include "key.h"
#include "uart.h"
#include "time.h"
#include "i2c.h"
#include "spi.h"
#include "w25q64.h"
#include "crc.h"

uint32_t buff[4] = {0x01020304, 0x05060708, 0x090A0B0C, 0x0D0E0F00};

int main(void)
{
	HAL_Init();
	RccClock_Init();
	UART1_Init(115200);
	CRC_Init();
	
	u1_printf("×Ö·û²âÊÔ\r\n");
	
	u1_printf("%x\r\n", HAL_CRC_Calculate(&crc, buff, 4));
	
	while (1)
	{
		
	}
}
