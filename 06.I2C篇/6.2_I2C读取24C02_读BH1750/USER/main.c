#include "stm32f1xx_hal.h"
#include "rcc.h"
#include "led.h"
#include "key.h"
#include "uart.h"
#include "time.h"
#include "i2c.h"

uint8_t wdata[8] = {0, 1, 2, 3, 4, 5, 6, 7};
uint8_t rdata[8];

uint8_t bh1750_cmd1 = 0x01; // 通电指令
uint8_t bh1750_cmd2 = 0x20; // 一次 H 分辨率模式指令
uint8_t bh1750_buff[2];     // 保存读取数据的缓冲区

int main(void)
{
	HAL_Init();
	RccClock_Init();
	LED_Init();
	UART1_Init(115200);
	I2C1_Init();
	
	u1_printf("字符测试\r\n");
	
	HAL_I2C_Mem_Write(&i2c1, 0xA0, 0, I2C_MEMADD_SIZE_8BIT, wdata, 8, 5000);
	if(HAL_I2C_IsDeviceReady(&i2c1, 0xA0, 50, 1000) == HAL_OK)
	{
		HAL_I2C_Mem_Read(&i2c1, 0xA0, 0, I2C_MEMADD_SIZE_8BIT, rdata, 8, 5000);
		for(uint8_t i = 0; i < 8; i++)
		{
			u1_printf("rdata[%d] : %d\r\n", i, rdata[i]);
		}
	}
	else
	{
		u1_printf("芯片忙碌\n");
	}
	
	
	while (1)
	{
		HAL_I2C_Master_Transmit(&i2c1, 0x46, &bh1750_cmd1, 1, 1000);
		HAL_Delay(5);
		HAL_I2C_Master_Transmit(&i2c1, 0x46, &bh1750_cmd2, 1, 1000);
		HAL_Delay(200);
		HAL_I2C_Master_Receive(&i2c1, 0x46, bh1750_buff, 2, 1000);
		u1_printf("光强度： %.2f Lux\n", ((bh1750_buff[0] * 256) + bh1750_buff[1]) / 1.2);
		HAL_Delay(1000);
	}
}
