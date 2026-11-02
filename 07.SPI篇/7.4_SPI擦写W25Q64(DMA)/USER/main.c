#include "stm32f1xx_hal.h"
#include "rcc.h"
#include "led.h"
#include "key.h"
#include "uart.h"
#include "time.h"
#include "i2c.h"
#include "spi.h"
#include "w25q64.h"

int main(void)
{
	HAL_Init();
	RccClock_Init();
	LED_Init();
	UART1_Init(921600);
	I2C1_Init();
	SPI1_Init();
	
	u1_printf("×Ö·û²âÊÔ\r\n");
	
	W25Q64_Sector_Erase(0);
//	for(uint8_t i = 0; i < 128; i++)
//	{
//		W25Q64_Block_Erase(i);
//	}
	
	for(uint16_t i = 0; i < 4096; i++)
	{
		wbuff[i] = 0x88;
	}

	for(uint16_t i = 0; i < 16; i++)
	{
		W25Q64_Page_Write(i, &wbuff[i * 256]);
	}
	
	W25Q64_Read_Data(0 * 4096, 4096, rbuff);
	
	while (1)
	{
		
	}
}
