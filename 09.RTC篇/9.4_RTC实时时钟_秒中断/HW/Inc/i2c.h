#ifndef __I2C_H
#define __I2C_H

#include "stm32f1xx_hal.h"

#define I2C1_SCL_PORT      GPIOB
#define I2C1_SCL_PIN       GPIO_PIN_6

#define I2C1_SDA_PORT      GPIOB
#define I2C1_SDA_PIN       GPIO_PIN_7

extern I2C_HandleTypeDef i2c1; // I2C1总控结构体

void I2C1_Init(void);

#endif /* __I2C_H */

