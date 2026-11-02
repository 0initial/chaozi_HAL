#ifndef __LED_H
#define __LED_H

/* LED 端口宏定义 */
#define LED_RED_PORT      GPIOB
#define LED_RED_PIN       GPIO_PIN_5

#define LED_GREEN_PORT    GPIOB
#define LED_GREEN_PIN     GPIO_PIN_0

#define LED_BLUE_PORT     GPIOB
#define LED_BLUE_PIN      GPIO_PIN_1

/* LED 状态宏定义 */
#define LED_RED_ENABLE()      HAL_GPIO_WritePin(LED_RED_PORT, LED_RED_PIN, GPIO_PIN_RESET)
#define LED_RED_DISABLE()     HAL_GPIO_WritePin(LED_RED_PORT, LED_RED_PIN, GPIO_PIN_SET)

#define LED_GREEN_ENABLE()    HAL_GPIO_WritePin(LED_GREEN_PORT, LED_GREEN_PIN, GPIO_PIN_RESET)
#define LED_GREEN_DISABLE()   HAL_GPIO_WritePin(LED_GREEN_PORT, LED_GREEN_PIN, GPIO_PIN_SET)

#define LED_BLUE_ENABLE()     HAL_GPIO_WritePin(LED_BLUE_PORT, LED_BLUE_PIN, GPIO_PIN_RESET)
#define LED_BLUE_DISABLE()    HAL_GPIO_WritePin(LED_BLUE_PORT, LED_BLUE_PIN, GPIO_PIN_SET)

void LED_Init(void);
void LED_GPIO_Init(void);
void LED_All_DISABLE(void);

void LED_Red(void);
void LED_Green(void);
void LED_Blue(void);
void LED_Yellow(void);
void LED_Purple(void);
void LED_Cyan(void);

#endif /* __LED_H */

