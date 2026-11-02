#ifndef __UART_H
#define __UART_H

extern UART_HandleTypeDef uart1;

#define UART1_TX_PORT   GPIOA
#define UART1_TX_PIN    GPIO_PIN_9

#define UART1_RX_PORT   GPIOA
#define UART1_RX_PIN    GPIO_PIN_10

void UART1_Init(uint32_t bandrate);

#endif /* __UART_H */
