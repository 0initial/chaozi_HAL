#include "stm32f1xx_hal.h"
#include "rcc.h"
#include "led.h"
#include "key.h"
#include "timer.h"
#include "uart.h"
#include "can.h"

uint8_t rxbuff[16];
uint8_t Current_KeyNum;

int main(void)
{
	HAL_Init();
	RccClock_Init();
//	LED_Init();
//	KEY_Init();
	UART1_Init(115200);
//	TIM2_Init(7200, 200); // 20ms
//	CAN1_Init();

	u1_printf("字符测试，设备2\r\n");
	
	while (1)
	{
//		Current_KeyNum = Key_GetNum();
//		if(Current_KeyNum == 1)
//		{
//		}
//		else if(Current_KeyNum == 2)
//		{
//		}
//		
		if(uart1.RxOutPtr != uart1.RxInPtr)
		{
			CAN_TxData(uart1.RxOutPtr->start, uart1.RxOutPtr->end - uart1.RxOutPtr->start + 1);
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
			// 根据项目情况，这个Out指针得考虑放进发送完成回调函数中，这样才能确保数据发送出去，以避免覆盖未发送的数据
			uart1.TxOutPtr++;
			if(uart1.TxOutPtr == uart1.TxEndPtr)
			{
				uart1.TxOutPtr = &uart1.TxLocation[0];
			}
		}

//		CAN_RxData(rxbuff);
	}
}
