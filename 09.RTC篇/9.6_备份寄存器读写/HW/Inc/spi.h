#ifndef __SPI_H
#define __SPI_H

#include "stm32f1xx_hal.h"

#define SPI1_CS_PORT      GPIOA
#define SPI1_CS_PIN       GPIO_PIN_4

#define SPI1_CLK_PORT     GPIOA
#define SPI1_CLK_PIN      GPIO_PIN_5

#define SPI1_MISO_PORT    GPIOA
#define SPI1_MISO_PIN     GPIO_PIN_6

#define SPI1_MOSI_PORT    GPIOA
#define SPI1_MOSI_PIN     GPIO_PIN_7

extern SPI_HandleTypeDef spi1;

void SPI1_Init(void);

#endif
