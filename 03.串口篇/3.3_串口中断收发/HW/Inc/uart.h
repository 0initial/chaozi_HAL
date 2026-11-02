#ifndef __UART_H
#define __UART_H

extern UART_HandleTypeDef uart1;
extern UART_HandleTypeDef uart2;
extern UART_HandleTypeDef uart3;

extern uint8_t txbuff[64], rxbuff[64];
extern uint8_t rxstate;

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

void UART1_Init(uint32_t bandrate);
void UART2_Init(uint32_t bandrate);
void UART3_Init(uint32_t bandrate);

#endif /* __UART_H */
