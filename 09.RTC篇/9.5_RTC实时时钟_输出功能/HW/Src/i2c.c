#include "i2c.h"
#include "uart.h"

I2C_HandleTypeDef i2c1; // I2C1总控结构体

// I2C初始化
void I2C1_Init(void)
{
	i2c1.Instance = I2C1;
	i2c1.Init.ClockSpeed = 100000;
	HAL_I2C_Init(&i2c1);
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
	}
}




