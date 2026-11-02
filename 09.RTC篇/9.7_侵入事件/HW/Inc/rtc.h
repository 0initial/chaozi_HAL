#ifndef __RTC_H
#define __RTC_H

#include "stm32f1xx_hal.h"

extern RTC_HandleTypeDef rtc; // RTC总控结构体

void RTC_Init(void);

#endif
