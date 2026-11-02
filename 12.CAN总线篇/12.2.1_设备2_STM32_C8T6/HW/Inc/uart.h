#ifndef __UART_H
#define __UART_H

#include "stm32f1xx_hal.h"

#define U1_TX_SIZE      2048
#define U1_RX_SIZE      2048
#define U1_RX_MAX       256

/************************ UART1 硬件宏定义 ************************/
#define UART1_TX_PORT   GPIOA
#define UART1_TX_PIN    GPIO_PIN_9

#define UART1_RX_PORT   GPIOA
#define UART1_RX_PIN    GPIO_PIN_10

/************************ UART 循环缓冲区结构体 ************************/
typedef struct
{
	uint8_t *start;
	uint8_t *end;
}LCB; // Location_Control_Block

typedef struct
{
	uint32_t RxCounter; // 接收数据量计数
	uint32_t TxCounter; // 发送数据量计数
	uint8_t  TxState;   // 发送标志位 0：发送空闲 1：发送忙
	LCB RxLocation[10]; // 每次接收的缓冲区，256 * 10 = 2048，所以这里设置10，可以根据总的缓冲区设置
	LCB TxLocation[10]; // 同上
	LCB *RxInPtr;       // 下次接受时，数据存放位置的开头
	LCB *RxOutPtr;      // 本次接收到数据的起始位置，如果要读取本次接收的数据，从这里开始
	LCB *RxEndPtr;      // 总缓冲区的末尾，不动
	LCB *TxInPtr;       // 发送数据时，将发送数据放入缓冲区的起始位置
	LCB *TxOutPtr;      // 发送时，当前发送数据的地址
	LCB *TxEndPtr;      // 总缓冲区的末尾，不动
	UART_HandleTypeDef uart; // HAL库官方串口结构体
	DMA_HandleTypeDef dmatx;
	DMA_HandleTypeDef dmarx;
}UCB; // Uart_Control_Blcok

extern UCB uart1;

void UART1_Init(uint32_t bandrate);
void U1_PtrInit(void);

void U1_TxData(uint8_t *data, uint32_t datasize);
void u1_printf(const char* format, ...);

#endif /* __UART_H */

