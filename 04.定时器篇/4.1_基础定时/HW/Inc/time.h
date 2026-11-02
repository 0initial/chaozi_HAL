#ifndef __TIME_H
#define __TIME_H

#include "stm32f1xx_hal.h"

extern TIM_HandleTypeDef tim1;

void Timer1_Init(uint16_t psc, uint16_t arr, uint8_t rep);

#endif /* __TIME_H */
