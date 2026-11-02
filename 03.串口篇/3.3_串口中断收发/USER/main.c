#include "stm32f1xx_hal.h"
#include "rcc.h"
#include "led.h"
#include "key.h"
#include "uart.h"

int main(void)
{
	HAL_Init();
	RccClock_Init();
	LED_Init();
	KEY_Init_IT(0);
	UART1_Init(115200);
	UART2_Init(115200);
	UART3_Init(115200);
	
	while (1)
	{
		if(rxstate == 1)
		{
			rxstate = 0;
			HAL_UART_Transmit_IT(&uart1, txbuff, 20);
		}
	}
}
