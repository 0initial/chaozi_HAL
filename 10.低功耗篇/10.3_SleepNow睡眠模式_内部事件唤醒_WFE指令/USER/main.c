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
	KEY_Init_IT();
	UART1_Init(115200);
	
	u1_printf("字符测试\r\n");
	
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
		
		Timer1_Init(36000, 4000, 0);
		
		HAL_PWR_EnableSEVOnPend();
		HAL_PWR_DisableSleepOnExit();
		
		// 关闭HAL库的时基中断，防止 Sleep-Now 睡眠模式被唤醒
		SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
		
		HAL_PWR_EnterSLEEPMode(0, PWR_SLEEPENTRY_WFE);
		
		u1_printf("唤醒\r\n");
		
		HAL_TIM_Base_Stop_IT(&tim1);
		__HAL_TIM_CLEAR_IT(&tim1, TIM_IT_UPDATE);
		NVIC_ClearPendingIRQ(TIM1_UP_TIM10_IRQn);
		// 唤醒后再开启HAL库时基，防止HAL库程序不运行
		SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
	}
}

