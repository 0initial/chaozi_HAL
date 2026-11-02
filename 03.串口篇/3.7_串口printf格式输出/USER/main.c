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
	
	u1_printf("%x %d %c\r\n", 0x30, 0x30, 0x30);
	
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
			HAL_UART_Transmit_DMA(&uart1.uart, uart1.TxOutPtr->start, uart1.TxOutPtr->end - uart1.TxOutPtr->start + 1);
			uart1.TxOutPtr++;
			if(uart1.TxOutPtr == uart1.TxEndPtr)
			{
				uart1.TxOutPtr = &uart1.TxLocation[0];
			}
		}

		if(uart2.RxOutPtr != uart2.RxInPtr)
		{
			U2_TxData(uart2.RxOutPtr->start, uart2.RxOutPtr->end - uart2.RxOutPtr->start + 1);
			uart2.RxOutPtr++;
			if(uart2.RxOutPtr == uart2.RxEndPtr)
			{
				uart2.RxOutPtr = &uart2.RxLocation[0];
			}
		}
		if((uart2.TxOutPtr != uart2.TxInPtr) && (uart2.TxState == 0))
		{
			uart2.TxState = 1;
			HAL_UART_Transmit_DMA(&uart2.uart, uart2.TxOutPtr->start, uart2.TxOutPtr->end - uart2.TxOutPtr->start + 1);
			uart2.TxOutPtr++;
			if(uart2.TxOutPtr == uart2.TxEndPtr)
			{
				uart2.TxOutPtr = &uart2.TxLocation[0];
			}
		}
	}
}
