#ifndef __TIME_H
#define __TIME_H

#include "stm32f1xx_hal.h"

#define TIM1_CH1_PORT    GPIOA
#define TIM1_CH1_PIN     GPIO_PIN_8

#define TIM1_CH2_PORT    GPIOA
#define TIM1_CH2_PIN     GPIO_PIN_9

extern TIM_HandleTypeDef tim1;

extern DMA_HandleTypeDef tim1_dmaup;

void Timer1_Init(uint16_t psc, uint16_t arr, uint8_t rep);

#endif /* __TIME_H */
