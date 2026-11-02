#include "stm32f1xx_hal.h"
#include "uart.h"

UART_HandleTypeDef uart1;

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
	}
}

