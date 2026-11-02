#include "stm32f1xx_hal.h"
#include "rcc.h"
#include "led.h"
#include "key.h"
#include "uart.h"
#include "time.h"
#include "i2c.h"

uint8_t bh1750_cmd1 = 0x01; // 通电指令
uint8_t bh1750_cmd2 = 0x20; // 一次 H 分辨率模式指令
uint8_t bh1750_buff[2];     // 保存读取数据的缓冲区

int main(void)
{
	HAL_Init();
	RccClock_Init();
	KEY_Init();
	UART1_Init(115200);
	I2C1_Init();
	
	u1_printf("字符测试\r\n");
	
	while (1)
	{
		switch(Key_Scan(0))
		{
			case 1:
				// 写入数据
				if(HAL_I2C_IsDeviceReady(&i2c1, 0xA0, 50, 1000) == HAL_OK)
				{
					HAL_I2C_Mem_Write_IT(&i2c1, 0xA0, 0, I2C_MEMADD_SIZE_8BIT, wdata, 8);
				}
				else
				{
					u1_printf("写入时芯片忙碌\n");
				}
				break;
			
			case 2:
				// 读取数据
				if(HAL_I2C_IsDeviceReady(&i2c1, 0xA0, 50, 1000) == HAL_OK)
				{
					HAL_I2C_Mem_Read_IT(&i2c1, 0xA0, 0, I2C_MEMADD_SIZE_8BIT, rdata, 8);
				}
				else
				{
					u1_printf("读取时芯片忙碌\n");
				}
				break;
		}
	}
}
