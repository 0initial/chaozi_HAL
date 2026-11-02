#include "string.h"
#include "stm32f1xx_hal.h"
#include "uart.h"

UCB uart1;
UCB uart2;
UCB uart3;

uint8_t U1_TxBuff[U1_TX_SIZE];
uint8_t U1_RxBuff[U1_RX_SIZE];

uint8_t U2_TxBuff[U2_TX_SIZE];
uint8_t U2_RxBuff[U2_RX_SIZE];

uint8_t U3_TxBuff[U3_TX_SIZE];
uint8_t U3_RxBuff[U3_RX_SIZE];

void UART1_Init(uint32_t bandrate)
{
	uart1.uart.Instance = USART1;
	uart1.uart.Init.BaudRate = bandrate;
	uart1.uart.Init.WordLength = UART_WORDLENGTH_8B;
	uart1.uart.Init.StopBits = UART_STOPBITS_1;
	uart1.uart.Init.Parity = UART_PARITY_NONE;
	uart1.uart.Init.Mode = UART_MODE_TX_RX;
	uart1.uart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	HAL_UART_Init(&uart1.uart);
	U1_PrtInit();
	
	// 开启空闲中断
	__HAL_UART_ENABLE_IT(&uart1.uart, UART_IT_IDLE);
	// 使能接收
	HAL_UART_Receive_DMA(&uart1.uart, uart1.RxInPtr->start, U1_RX_MAX);
}

// 串口1缓冲区指针初始化
void U1_PrtInit(void)
{
	uart1.RxInPtr = uart1.RxLocation;
	uart1.RxOutPtr = &uart1.RxLocation[0];
	uart1.RxEndPtr = &uart1.RxLocation[9];
	uart1.RxCounter = 0;
	uart1.RxInPtr->start = U1_RxBuff;
	
	uart1.TxState = 0;
	uart1.TxInPtr = uart1.TxLocation;
	uart1.TxOutPtr = &uart1.TxLocation[0];
	uart1.TxEndPtr = &uart1.TxLocation[9];
	uart1.TxCounter = 0;
	uart1.TxInPtr->start = U1_TxBuff;
}

// 串口1发送数据函数
void U1_TxData(uint8_t *data, uint32_t datasize)
{
	// 如果发送缓冲区剩余位置还够放下本次发送的数据长度
	if((U1_TX_SIZE - uart1.TxCounter) >= datasize)
	{
		uart1.TxInPtr->start = &U1_TxBuff[uart1.TxCounter];
	}
	else
	{
		uart1.TxCounter = 0;
		uart1.TxInPtr->start = U1_TxBuff;
	}
	memcpy(uart1.TxInPtr->start, data, datasize);
	uart1.TxCounter += datasize;
	uart1.TxInPtr->end = &U1_TxBuff[uart1.TxCounter - 1];
	uart1.TxInPtr++;
	if(uart1.TxInPtr == uart1.TxEndPtr)
	{
		uart1.TxInPtr = uart1.TxLocation;
	}
}

void UART2_Init(uint32_t bandrate)
{
	uart2.uart.Instance = USART2;
	uart2.uart.Init.BaudRate = bandrate;
	uart2.uart.Init.WordLength = UART_WORDLENGTH_8B;
	uart2.uart.Init.StopBits = UART_STOPBITS_1;
	uart2.uart.Init.Parity = UART_PARITY_NONE;
	uart2.uart.Init.Mode = UART_MODE_TX_RX;
	uart2.uart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	HAL_UART_Init(&uart2.uart);
	U2_PrtInit();
	
	// 开启空闲中断
	__HAL_UART_ENABLE_IT(&uart2.uart, UART_IT_IDLE);
	// 使能接收
	HAL_UART_Receive_IT(&uart2.uart, uart2.RxInPtr->start, U2_RX_MAX);
}

void U2_PrtInit(void)
{
	uart2.RxInPtr = &uart2.RxLocation[0];
	uart2.RxOutPtr = &uart2.RxLocation[0];
	uart2.RxEndPtr = &uart2.RxLocation[9];
	uart2.RxCounter = 0;
	uart2.RxInPtr->start = U2_RxBuff;

	uart2.TxState = 0;
	uart2.TxInPtr = &uart2.TxLocation[0];
	uart2.TxOutPtr = &uart2.TxLocation[0];
	uart2.TxEndPtr = &uart2.TxLocation[9];
	uart2.TxCounter = 0;
	uart2.TxInPtr->start = U2_TxBuff;
}

// 串口2发送数据函数
void U2_TxData(uint8_t *data, uint32_t datasize)
{
	// 如果发送缓冲区剩余位置还够放下本次发送的数据长度
	if((U2_TX_SIZE - uart2.TxCounter) >= datasize)
	{
		uart2.TxInPtr->start = &U2_TxBuff[uart2.TxCounter];
	}
	else
	{
		uart2.TxCounter = 0;
		uart2.TxInPtr->start = U2_TxBuff;
	}
	memcpy(uart2.TxInPtr->start, data, datasize);
	uart2.TxCounter += datasize;
	uart2.TxInPtr->end = &U2_TxBuff[uart2.TxCounter - 1];
	uart2.TxInPtr++;
	if(uart2.TxInPtr == uart2.TxEndPtr)
	{
		uart2.TxInPtr = uart2.TxLocation;
	}
}

void UART3_Init(uint32_t bandrate)
{
	uart3.uart.Instance = USART3;
	uart3.uart.Init.BaudRate = bandrate;
	uart3.uart.Init.WordLength = UART_WORDLENGTH_8B;
	uart3.uart.Init.StopBits = UART_STOPBITS_1;
	uart3.uart.Init.Parity = UART_PARITY_NONE;
	uart3.uart.Init.Mode = UART_MODE_TX_RX;
	uart3.uart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	HAL_UART_Init(&uart3.uart);
	U3_PrtInit();
	
	// 开启空闲中断
	__HAL_UART_ENABLE_IT(&uart3.uart, UART_IT_IDLE);
	// 使能接收
	HAL_UART_Receive_IT(&uart3.uart, uart3.RxInPtr->start, U3_RX_MAX);
}

void U3_PrtInit(void)
{
	uart3.RxInPtr = &uart3.RxLocation[0];
	uart3.RxOutPtr = &uart3.RxLocation[0];
	uart3.RxEndPtr = &uart3.RxLocation[9];
	uart3.RxCounter = 0;
	uart3.RxInPtr->start = U3_RxBuff;

	uart3.TxState = 0;
	uart3.TxInPtr = &uart3.TxLocation[0];
	uart3.TxOutPtr = &uart3.TxLocation[0];
	uart3.TxEndPtr = &uart3.TxLocation[9];
	uart3.TxCounter = 0;
	uart3.TxInPtr->start = U3_TxBuff;
}

// 串口3发送数据函数
void U3_TxData(uint8_t *data, uint32_t datasize)
{
	// 如果发送缓冲区剩余位置还够放下本次发送的数据长度
	if((U3_TX_SIZE - uart3.TxCounter) >= datasize)
	{
		uart3.TxInPtr->start = &U3_TxBuff[uart3.TxCounter];
	}
	else
	{
		uart3.TxCounter = 0;
		uart3.TxInPtr->start = U3_TxBuff;
	}
	memcpy(uart3.TxInPtr->start, data, datasize);
	uart3.TxCounter += datasize;
	uart3.TxInPtr->end = &U3_TxBuff[uart3.TxCounter - 1];
	uart3.TxInPtr++;
	if(uart3.TxInPtr == uart3.TxEndPtr)
	{
		uart3.TxInPtr = uart3.TxLocation;
	}
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	GPIO_InitTypeDef GPIO_InitTypeStruct;
	
	if(huart->Instance == USART1)
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_USART1_CLK_ENABLE();
		__HAL_RCC_DMA1_CLK_ENABLE();

		// UART1 TX
		GPIO_InitTypeStruct.Pin = UART1_TX_PIN;
		GPIO_InitTypeStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitTypeStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
		HAL_GPIO_Init(UART1_TX_PORT, &GPIO_InitTypeStruct);

		// UART1 RX
		GPIO_InitTypeStruct.Pin = UART1_RX_PIN;
		GPIO_InitTypeStruct.Mode = GPIO_MODE_AF_INPUT;
		GPIO_InitTypeStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(UART1_RX_PORT, &GPIO_InitTypeStruct);
		
		// DMA配置 Tx
		uart1.dmatx.Instance = DMA1_Channel4;
		uart1.dmatx.Init.Direction = DMA_MEMORY_TO_PERIPH;
		uart1.dmatx.Init.PeriphInc = DMA_PINC_DISABLE;
		uart1.dmatx.Init.MemInc = DMA_MINC_ENABLE;
		uart1.dmatx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		uart1.dmatx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
		uart1.dmatx.Init.Mode = DMA_NORMAL;
		uart1.dmatx.Init.Priority = DMA_PRIORITY_MEDIUM;
		__HAL_LINKDMA(huart, hdmatx, uart1.dmatx);
		HAL_DMA_Init(&uart1.dmatx);
		
		// Rx
		uart1.dmarx.Instance = DMA1_Channel5;
		uart1.dmarx.Init.Direction = DMA_PERIPH_TO_MEMORY;
		uart1.dmarx.Init.PeriphInc = DMA_PINC_DISABLE;
		uart1.dmarx.Init.MemInc = DMA_MINC_ENABLE;
		uart1.dmarx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		uart1.dmarx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
		uart1.dmarx.Init.Mode = DMA_NORMAL;
		uart1.dmarx.Init.Priority = DMA_PRIORITY_MEDIUM;
		__HAL_LINKDMA(huart, hdmarx, uart1.dmarx);
		HAL_DMA_Init(&uart1.dmarx);
		
		// 开启中断
		HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(USART1_IRQn);
		
		HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);
		
		HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);
	}
	else if(huart->Instance == USART2)
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_USART2_CLK_ENABLE();
		
		// UART2 TX
		GPIO_InitTypeStruct.Pin = UART2_TX_PIN;
		GPIO_InitTypeStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitTypeStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
		HAL_GPIO_Init(UART2_TX_PORT, &GPIO_InitTypeStruct);

		// UART2 RX
		GPIO_InitTypeStruct.Pin = UART2_RX_PIN;
		GPIO_InitTypeStruct.Mode = GPIO_MODE_AF_INPUT;
		GPIO_InitTypeStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(UART2_RX_PORT, &GPIO_InitTypeStruct);
		
		// 开启中断
		HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(USART2_IRQn);
	}
	else if(huart->Instance == USART3)
	{
		__HAL_RCC_GPIOB_CLK_ENABLE();
		__HAL_RCC_USART3_CLK_ENABLE();
		
		// UART3 TX
		GPIO_InitTypeStruct.Pin = UART3_TX_PIN;
		GPIO_InitTypeStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitTypeStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
		HAL_GPIO_Init(UART3_TX_PORT, &GPIO_InitTypeStruct);

		// UART3 RX
		GPIO_InitTypeStruct.Pin = UART3_RX_PIN;
		GPIO_InitTypeStruct.Mode = GPIO_MODE_AF_INPUT;
		GPIO_InitTypeStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(UART3_RX_PORT, &GPIO_InitTypeStruct);
		
		// 开启中断
		HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(USART3_IRQn);
	}
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1)
	{
		uart1.TxState = 0;
	}
	else if(huart->Instance == USART2)
	{
		uart2.TxState = 0;
	}
	else if(huart->Instance == USART3)
	{
		uart3.TxState = 0;
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1)
	{
		
	}
}

void HAL_UART_AbortReceiveCpltCallback(UART_HandleTypeDef *huart)
{
	// 接收完成后，处理的是 In 指针，为的是下次接收数据的时候，指针指向正确的位置，Out 指针是处理发送用的
	if(huart->Instance == USART1)
	{
		// 接收完成后，标记本次接收的最后数据地址
		uart1.RxInPtr->end = &U1_RxBuff[uart1.RxCounter - 1];
		uart1.RxInPtr++;
		if(uart1.RxInPtr == uart1.RxEndPtr)
		{
			uart1.RxInPtr = &uart1.RxLocation[0];
		}
		if((U1_RX_SIZE - uart1.RxCounter) < U1_RX_MAX)
		{
			uart1.RxCounter = 0;
			uart1.RxInPtr->start = U1_RxBuff; 
		}
		else
		{
			uart1.RxInPtr->start = &U1_RxBuff[uart1.RxCounter];
		}
		
		// 使能接收
		HAL_UART_Receive_DMA(&uart1.uart, uart1.RxInPtr->start, U1_RX_MAX);
	}
	else if(huart->Instance == USART2)
	{
		// 接收完成后，标记本次接收的最后数据地址
		uart2.RxInPtr->end = &U2_RxBuff[uart2.RxCounter - 1];
		uart2.RxInPtr++;
		if(uart2.RxInPtr == uart2.RxEndPtr)
		{
			uart2.RxInPtr = &uart2.RxLocation[0];
		}
		if((U2_RX_SIZE - uart2.RxCounter) < U2_RX_MAX)
		{
			uart2.RxCounter = 0;
			uart2.RxInPtr->start = U2_RxBuff; 
		}
		else
		{
			uart2.RxInPtr->start = &U2_RxBuff[uart2.RxCounter];
		}
		
		// 使能接收
		HAL_UART_Receive_IT(&uart2.uart, uart2.RxInPtr->start, U2_RX_MAX);
	}
	else if(huart->Instance == USART3)
	{
		// 接收完成后，标记本次接收的最后数据地址
		uart3.RxInPtr->end = &U3_RxBuff[uart3.RxCounter - 1];
		uart3.RxInPtr++;
		if(uart3.RxInPtr == uart3.RxEndPtr)
		{
			uart3.RxInPtr = &uart3.RxLocation[0];
		}
		if((U3_RX_SIZE - uart3.RxCounter) < U3_RX_MAX)
		{
			uart3.RxCounter = 0;
			uart3.RxInPtr->start = U3_RxBuff; 
		}
		else
		{
			uart3.RxInPtr->start = &U3_RxBuff[uart3.RxCounter];
		}
		
		// 使能接收
		HAL_UART_Receive_IT(&uart3.uart, uart3.RxInPtr->start, U3_RX_MAX);
	}
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1)
	{
		
	}
}

