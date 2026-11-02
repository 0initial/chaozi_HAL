#ifndef __TIMER_H
#define __TIMER_H

#include "stm32f1xx_hal.h"

extern TIM_HandleTypeDef tim2;

void TIM2_Init(uint16_t psc, uint16_t arr);

#endif /* __TIMER_H */

