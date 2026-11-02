#include "stm32f1xx_hal.h"

#include "rcc.h"
#include "led.h"
#include "key.h"
#include "uart.h"
#include "timer.h"
#include "i2c.h"
#include "spi.h"
#include "crc.h"
#include "rtc.h"

#include "w25q64.h"

int main(void)
{
	HAL_Init();
	RccClock_Init();
	
	__HAL_RCC_PWR_CLK_ENABLE();
	
	KEY_Init_IT();
	UART1_Init(115200);
//	Timer1_Init(36000, 10000, 0);
	
	u1_printf("字符测试\r\n");
	
	if(__HAL_PWR_GET_FLAG(PWR_FLAG_SB))
	{
		u1_printf("从待机模式唤醒\r\n");
		__HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);
	}
	else
	{
		u1_printf("正常复位\r\n");
	}
	
	while (1)
	{
		u1_printf("5\r\n");
		HAL_Delay(1000);
		u1_printf("4\r\n");
		HAL_Delay(1000);
		u1_printf("3\r\n");
		HAL_Delay(1000);
		u1_printf("2\r\n");
		HAL_Delay(1000);
		u1_printf("1\r\n");
		HAL_Delay(1000);
		
		HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);
		__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
		HAL_PWR_EnterSTANDBYMode();
	}
}

