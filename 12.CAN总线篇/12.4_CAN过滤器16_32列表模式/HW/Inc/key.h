#ifndef __KEY_H
#define __KEY_H

#include "stm32f1xx_hal.h"

/* KEY 端口宏定义 */
#define KEY1_PROT   GPIOA
#define KEY1_PIN    GPIO_PIN_0

#define KEY2_PROT   GPIOC
#define KEY2_PIN    GPIO_PIN_13

/* KEY 状态宏定义 */
#define KEY1_READ()     HAL_GPIO_ReadPin(KEY1_PROT, KEY1_PIN)
#define KEY2_READ()     HAL_GPIO_ReadPin(KEY2_PROT, KEY2_PIN)

void KEY_Init(void);
uint8_t Key_GetNum(void);
uint8_t Key_Scan(void);
void Key_Tick(void);

#endif /* __KEY_H */

