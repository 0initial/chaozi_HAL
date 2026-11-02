#include "i2c.h"
#include "uart.h"

I2C_HandleTypeDef i2c1; // I2C1总控结构体

uint8_t wdata[8] = {0, 1, 2, 3, 4, 5, 6, 7};
uint8_t rdata[8];

// I2C初始化
void I2C1_Init(void)
{
	i2c1.Instance = I2C1;                               // 使用I2C1
	i2c1.Init.ClockSpeed = 100000;                      // 通信速率100K
	i2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT; // 7位寻址模式
	HAL_I2C_Init(&i2c1);                                // 初始化I2C1
}

void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)
{
	GPIO_InitTypeDef GPIO_InitTypeStruct;
	
	if(hi2c->Instance == I2C1)
	{
		__HAL_RCC_GPIOB_CLK_ENABLE();
		__HAL_RCC_I2C1_CLK_ENABLE();
		
		GPIO_InitTypeStruct.Pin = I2C1_SCL_PIN;
		GPIO_InitTypeStruct.Mode = GPIO_MODE_AF_OD;
		GPIO_InitTypeStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(I2C1_SCL_PORT, &GPIO_InitTypeStruct);
		
		GPIO_InitTypeStruct.Pin = I2C1_SDA_PIN;
		GPIO_InitTypeStruct.Mode = GPIO_MODE_AF_OD;
		GPIO_InitTypeStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(I2C1_SDA_PORT, &GPIO_InitTypeStruct);
		
		HAL_NVIC_SetPriority(I2C1_EV_IRQn, 1, 0);
		HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
	}
}

// I2C发送完成回调函数
void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	if(hi2c->Instance == I2C1)
	{
		u1_printf("写入完成\r\n");
	}
}

// I2C读取完成回调函数
void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	if(hi2c->Instance == I2C1)
	{
		u1_printf("读取完成\r\n");
		for(uint8_t i = 0; i < 8; i++)
		{
			u1_printf("rdata[%d] : %d\r\n", i, rdata[i]);
			wdata[i] += 8;
		}
	}
}



