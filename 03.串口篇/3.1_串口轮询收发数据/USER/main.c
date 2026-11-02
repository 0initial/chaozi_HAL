#include "stm32f1xx_hal.h"
#include "rcc.h"
#include "led.h"
#include "key.h"
#include "uart.h"

#define RX_SIZE   200

uint8_t buff[256];

int main(void)
{
	HAL_Init();
	RccClock_Init();
	LED_Init();
	KEY_Init_IT(0);
	UART1_Init(115200);
	
	while (1)
	{
		switch(HAL_UART_Receive(&uart1, buff, RX_SIZE, 200))
		{
			case HAL_OK:
				HAL_UART_Transmit(&uart1, buff, RX_SIZE, 200);
				break;
			
			case HAL_TIMEOUT:
				if(uart1.RxXferCount != (RX_SIZE - 1))
				{
					HAL_UART_Transmit(&uart1, buff, (RX_SIZE - 1 - uart1.RxXferCount), 200);
				}
				break;
				
			case HAL_BUSY:
				break;
			
			case HAL_ERROR:
				break;
		}
	}
}
