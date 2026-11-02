#ifndef __ADC_H
#define __ADC_H

#include "stm32f1xx_hal.h"

#define ADC1_CHANNEL1_PORT      GPIOA
#define ADC1_CHANNEL1_PIN       GPIO_PIN_0

extern ADC_HandleTypeDef  adc1; // 外部变量声明

void ADC_Init(void);

#endif /* __ADC_H */

