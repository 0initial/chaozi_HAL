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
		if(uart1.RxOutPtr != uart1.RxInPtr)
		{
			U1_TxData(uart1.RxOutPtr->start, uart1.RxOutPtr->end - uart1.RxOutPtr->start + 1);
			uart1.RxOutPtr++;
			if(uart1.RxOutPtr == uart1.RxEndPtr)
			{
				uart1.RxOutPtr = &uart1.RxLocation[0];
			}
		}
		if((uart1.TxOutPtr != uart1.TxInPtr) && (uart1.TxState == 0))
		{
			uart1.TxState = 1;
			HAL_UART_Transmit_IT(&uart1.uart, uart1.TxOutPtr->start, uart1.TxOutPtr->end - uart1.TxOutPtr->start + 1);
			uart1.TxOutPtr++;
			if(uart1.TxOutPtr == uart1.TxEndPtr)
			{
				uart1.TxOutPtr = &uart1.TxLocation[0];
			}
		}
	}
}
