#ifndef __KEY_H
#define __KEY_H

#include "stm32f1xx_hal.h"

/* KEY 端口宏定义 */
#define KEY1_PROT   GPIOA
#define KEY1_PIN    GPIO_PIN_0

#define KEY2_PROT   GPIOC
#define KEY2_PIN    GPIO_PIN_13

/* KEY 状态宏定义 */
#define KEY1_IN()     HAL_GPIO_ReadPin(KEY1_PROT, KEY1_PIN)
#define KEY2_IN()     HAL_GPIO_ReadPin(KEY2_PROT, KEY2_PIN)

void KEY_Init(void);
uint8_t Key_Scan(uint8_t mode);
void KEY_Init_IT(uint8_t mode);
void KEY_Init_Event(void);
void KEY_Init_EventOut(void);

#endif /* __KEY_H */

