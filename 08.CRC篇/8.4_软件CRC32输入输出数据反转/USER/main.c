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
uint8_t buff2[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0};

int main(void)
{
	HAL_Init();
	RccClock_Init();
	UART1_Init(115200);
	CRC_Init();
	
	u1_printf("字符测试\r\n");
	
	u1_printf("单次校验结果：%x\r\n", HAL_CRC_Calculate(&crc, buff, 4));
	
	HAL_CRC_Calculate(&crc, &buff[0], 1);
	HAL_CRC_Accumulate(&crc, &buff[1], 1);
	HAL_CRC_Accumulate(&crc, &buff[2], 1);
	u1_printf("多次连续校验结果：%x\r\n", HAL_CRC_Accumulate(&crc, &buff[3], 1));
	
	u1_printf("软件校验结果：%x\r\n", CRC32(buff2, 16, 0x87654321));
	
	while (1)
	{
		
	}
}

