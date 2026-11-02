#include "time.h"

#include "rtc.h"
#include "uart.h"
#include "led.h"

RTC_HandleTypeDef rtc; // RTC总控结构体
struct tm timedata;
struct tm alarmdata;

time_t temp;

// RTC初始化
void RTC_Init(void)
{
	rtc.Instance = RTC;                        // 配置RTC
	rtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND; // 自动配置分频值 1秒1个数
	rtc.Init.OutPut = RTC_OUTPUTSOURCE_SECOND;   // 不使用输出功能
	HAL_RTC_Init(&rtc);
	
	timedata.tm_year = 2024 - 1900;
	timedata.tm_mon = 1;
	timedata.tm_mday = 1;
	timedata.tm_hour = 0;
	timedata.tm_min = 0;
	timedata.tm_sec = 0;
	
	alarmdata.tm_year = 2024 - 1900;
	alarmdata.tm_mon = 1;
	alarmdata.tm_mday = 1;
	alarmdata.tm_hour = 0;
	alarmdata.tm_min = 0;
	alarmdata.tm_sec = 5;
	
	temp = mktime(&timedata);
	// 关闭写保护
	__HAL_RTC_WRITEPROTECTION_DISABLE(&rtc);
	// 设置起始时间
	WRITE_REG(rtc.Instance->CNTH, temp >> 16);
	WRITE_REG(rtc.Instance->CNTL, temp & 0x0000FFFF);
	
	temp = mktime(&alarmdata);
	// 设置闹钟时间
	WRITE_REG(rtc.Instance->ALRH, temp >> 16);
	WRITE_REG(rtc.Instance->ALRL, temp & 0x0000FFFF);
	// 开启写保护
	__HAL_RTC_WRITEPROTECTION_ENABLE(&rtc);
	
	// 写保护后加点延迟，防止寄存器出错
	HAL_Delay(20);
	
   __HAL_RTC_ALARM_CLEAR_FLAG(&rtc, RTC_FLAG_ALRAF);
   __HAL_RTC_ALARM_ENABLE_IT(&rtc, RTC_IT_ALRA);
   __HAL_RTC_ALARM_EXTI_ENABLE_IT();
   __HAL_RTC_ALARM_EXTI_ENABLE_RISING_EDGE();
	 
	 HAL_RTCEx_SetSecond_IT(&rtc);
}

// RTC初始化 硬件配置回调函数
void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc)
{
	__HAL_RCC_RTC_ENABLE();
	__HAL_RCC_BKP_CLK_ENABLE();
	
	HAL_NVIC_SetPriority(RTC_Alarm_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);
	
	HAL_NVIC_SetPriority(RTC_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ(RTC_IRQn);
}

struct tm *GetTimeData(void)
{
	temp = (READ_REG(rtc.Instance->CNTH) << 16) | (READ_REG(rtc.Instance->CNTL) & 0x0000FFFF);
	return localtime(&temp);
}

// 闹钟中断回调函数
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
	u1_printf("闹钟A时间到\r\n");
}

// RTC 秒中断回调函数
void HAL_RTCEx_RTCEventCallback(RTC_HandleTypeDef *hrtc)
{
//	HAL_GPIO_TogglePin(LED_GREEN_PORT, LED_GREEN_PIN);
}

