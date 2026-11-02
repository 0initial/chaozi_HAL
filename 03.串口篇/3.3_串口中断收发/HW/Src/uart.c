#include "string.h"
#include "stm32f1xx_hal.h"
#include "uart.h"

UART_HandleTypeDef uart1;
UART_HandleTypeDef uart2;
UART_HandleTypeDef uart3;

uint8_t txbuff[64], rxbuff[64];
uint8_t rxstate;

void UART1_Init(uint32_t bandrate)
{
	uart1.Instance = USART1;
	uart1.Init.BaudRate = bandrate;
	uart1.Init.WordLength = UART_WORDLENGTH_8B;
	uart1.Init.StopBits = UART_STOPBITS_1;
	uart1.Init.Parity = UART_PARITY_NONE;
	uart1.Init.Mode = UART_MODE_TX_RX;
	uart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	HAL_UART_Init(&uart1);
	
	// 开启中断接收
	HAL_UART_Receive_IT(&uart1, rxbuff, 20);
}

void UART2_Init(uint32_t bandrate)
{
	uart2.Instance = USART2;
	uart2.Init.BaudRate = bandrate;
	uart2.Init.WordLength = UART_WORDLENGTH_8B;
	uart2.Init.StopBits = UART_STOPBITS_1;
	uart2.Init.Parity = UART_PARITY_NONE;
	uart2.Init.Mode = UART_MODE_TX_RX;
	uart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	HAL_UART_Init(&uart2);
}

void UART3_Init(uint32_t bandrate)
{
	uart3.Instance = USART3;
	uart3.Init.BaudRate = bandrate;
	uart3.Init.WordLength = UART_WORDLENGTH_8B;
	uart3.Init.StopBits = UART_STOPBITS_1;
	uart3.Init.Parity = UART_PARITY_NONE;
	uart3.Init.Mode = UART_MODE_TX_RX;
	uart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	HAL_UART_Init(&uart3);
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	GPIO_InitTypeDef GPIO_InitTypeStruct;
	
	if(huart->Instance == USART1)
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_USART1_CLK_ENABLE();

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
		
		// 开启中断
		HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(USART1_IRQn);
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
	}
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1)
	{
		// 将接收的数据复制到发送数据
		memcpy(txbuff, rxbuff, 20);
		rxstate = 1;
		HAL_UART_Receive_IT(&uart1, rxbuff, 20);
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1)
	{
		// 将接收的数据复制到发送数据
		memcpy(txbuff, rxbuff, 20);
		rxstate = 1;
		HAL_UART_Receive_IT(&uart1, rxbuff, 20);
	}
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1)
	{
		
	}
}

