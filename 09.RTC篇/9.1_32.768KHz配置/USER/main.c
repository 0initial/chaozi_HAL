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

int main(void)
{
	HAL_Init();
	RccClock_Init();
	UART1_Init(115200);
	CRC_Init();
	
	u1_printf("×Ö·û²âÊÔ\r\n");
	
	while (1)
	{
		
	}
}

