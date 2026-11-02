#ifndef __TIME_H
#define __TIME_H

#include "stm32f1xx_hal.h"

#define TIM1_CH1_PORT    GPIOA
#define TIM1_CH1_PIN     GPIO_PIN_8

#define TIM1_CH1N_PORT   GPIOB
#define TIM1_CH1N_PIN    GPIO_PIN_13

#define TIM1_CH2_PORT    GPIOA
#define TIM1_CH2_PIN     GPIO_PIN_9

#define TIM1_CH3_PORT    GPIOA
#define TIM1_CH3_PIN     GPIO_PIN_10

#define TIM1_BKIN_PORT   GPIOB
#define TIM1_BKIN_PIN    GPIO_PIN_12

extern TIM_HandleTypeDef tim1;

void Timer1_Init(uint16_t psc, uint16_t arr, uint8_t rep);

#endif /* __TIME_H */
