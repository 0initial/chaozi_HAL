#ifndef __WDG_H
#define __WDG_H

#include "stm32f1xx_hal.h"

extern IWDG_HandleTypeDef iwdg; // 独立看门狗总控
extern WWDG_HandleTypeDef wwdg; // 窗口看门狗总控

void IWDG_Init(void);
void WWDG_Init(void);

#endif /* __WDG_H */

