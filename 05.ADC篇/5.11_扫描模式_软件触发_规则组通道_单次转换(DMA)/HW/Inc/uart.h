#ifndef __UART_H
#define __UART_H

#include "stm32f1xx_hal.h"
#include "stdio.h"
#include "string.h"
#include "stdarg.h"

#define U1_RX_SIZE      2048
#define U1_TX_SIZE      2048
#define U1_RX_MAX       256

#define U2_RX_SIZE      2048
#define U2_TX_SIZE      2048
#define U2_RX_MAX       256

#define U3_RX_SIZE      2048
#define U3_TX_SIZE      2048
#define U3_RX_MAX       256

/* UART 硬件宏定义 */
#define UART1_TX_PORT   GPIOA
#define UART1_TX_PIN    GPIO_PIN_9

#define UART1_RX_PORT   GPIOA
#define UART1_RX_PIN    GPIO_PIN_10

#define UART2_TX_PORT   GPIOA
#define UART2_TX_PIN    GPIO_PIN_2

#define UART2_RX_PORT   GPIOA
#define UART2_RX_PIN    GPIO_PIN_3

#define UART3_TX_PORT   GPIOB
#define UART3_TX_PIN    GPIO_PIN_10

#define UART3_RX_PORT   GPIOB
#define UART3_RX_PIN    GPIO_PIN_11

typedef struct
{
	uint8_t *start;
	uint8_t *end;
}LCB; // Location_Control_Block

typedef struct
{
	uint32_t RxCounter;
	uint32_t TxCounter;
	uint8_t  TxState;
	LCB      RxLocation[10];
	LCB      TxLocation[10];
	LCB      *RxInPtr;
	LCB      *RxOutPtr;
	LCB      *RxEndPtr;
	LCB      *TxInPtr;
	LCB      *TxOutPtr;
	LCB      *TxEndPtr;
	UART_HandleTypeDef uart; // HAL库官方串口结构体
	DMA_HandleTypeDef dmatx;
	DMA_HandleTypeDef dmarx;
}UCB; // UART_Control_Block

extern UCB uart1;
extern UCB uart2;
extern UCB uart3;


void UART1_Init(uint32_t bandrate);
void UART2_Init(uint32_t bandrate);
void UART3_Init(uint32_t bandrate);

void U1_PrtInit(void);
void U2_PrtInit(void);
void U3_PrtInit(void);
void U1_TxData(uint8_t *data, uint32_t datasize);
void U2_TxData(uint8_t *data, uint32_t datasize);
void U3_TxData(uint8_t *data, uint32_t datasize);

void u1_printf(const char* format, ...);

#endif /* __UART_H */
