#ifndef __ADC_H
#define __ADC_H

#include "stm32f1xx_hal.h"

#define ADC1_CHANNEL11_PORT      GPIOC
#define ADC1_CHANNEL11_PIN       GPIO_PIN_1

extern ADC_HandleTypeDef  adc1; // 外部变量声明

void ADC_Init(void);

#endif /* __ADC_H */

