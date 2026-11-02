#include "rtc.h"
#include "uart.h"
#include "led.h"

RTC_HandleTypeDef rtc;    // RTC总控结构体
RTC_TamperTypeDef tamper; // 侵入事件总控结构体

// RTC初始化
void RTC_Init(void)
{
	rtc.Instance = RTC;                        // 配置RTC
	HAL_RTC_Init(&rtc);
	
	tamper.Tamper = RTC_TAMPER_1;                       // 选择侵入引脚 就一个PC13
	tamper.Trigger = RTC_TAMPERTRIGGER_HIGHLEVEL;       // 高有效
	__HAL_RTC_TAMPER_CLEAR_FLAG(&rtc, RTC_FLAG_TAMP1F); // 清除标志位
	HAL_RTCEx_SetTamper_IT(&rtc, &tamper);              // 配置侵入事件
}

// RTC初始化 硬件配置回调函数
void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc)
{
	__HAL_RCC_RTC_ENABLE();
	__HAL_RCC_BKP_CLK_ENABLE();
	
	HAL_NVIC_SetPriority(TAMPER_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ(TAMPER_IRQn);
}

// 侵入中断回调函数
void HAL_RTCEx_Tamper1EventCallback(RTC_HandleTypeDef *hrtc)
{
	HAL_RTCEx_DeactivateTamper(hrtc, RTC_FLAG_TAMP1F);
	u1_printf("发生侵入事件\r\n");
}



