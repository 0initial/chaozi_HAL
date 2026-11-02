#include "rtc.h"
#include "time.h"

RTC_HandleTypeDef rtc; // RTC总控结构体
struct tm timedata;

time_t temp;

// RTC初始化
void RTC_Init(void)
{
	rtc.Instance = RTC;                        // 配置RTC
	rtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND; // 自动配置分频值 1秒1个数
	rtc.Init.OutPut = RTC_OUTPUTSOURCE_NONE;   // 不使用输出功能
	HAL_RTC_Init(&rtc);
	
	timedata.tm_year = 2024 - 1900;
	timedata.tm_mon = 1;
	timedata.tm_mday = 1;
	timedata.tm_hour = 0;
	timedata.tm_min = 0;
	timedata.tm_sec = 0;
	
	temp = mktime(&timedata);
	__HAL_RTC_WRITEPROTECTION_DISABLE(&rtc);
	WRITE_REG(rtc.Instance->CNTH, temp >> 16);
	WRITE_REG(rtc.Instance->CNTL, temp & 0x0000FFFF);
	__HAL_RTC_WRITEPROTECTION_ENABLE(&rtc);
}

// RTC初始化 硬件配置回调函数
void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc)
{
	__HAL_RCC_RTC_ENABLE();
	__HAL_RCC_BKP_CLK_ENABLE();
}

struct tm *GetTimeData(void)
{
	temp = (READ_REG(rtc.Instance->CNTH) << 16) | (READ_REG(rtc.Instance->CNTL) & 0x0000FFFF);
	return localtime(&temp);
}



