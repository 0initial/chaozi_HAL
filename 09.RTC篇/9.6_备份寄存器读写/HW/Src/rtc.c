#include "rtc.h"
#include "uart.h"
#include "led.h"

RTC_HandleTypeDef rtc; // RTC总控结构体

// RTC初始化
void RTC_Init(void)
{
	rtc.Instance = RTC;                        // 配置RTC
	HAL_RTC_Init(&rtc);
}

// RTC初始化 硬件配置回调函数
void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc)
{
	__HAL_RCC_RTC_ENABLE();
	__HAL_RCC_BKP_CLK_ENABLE();
}

