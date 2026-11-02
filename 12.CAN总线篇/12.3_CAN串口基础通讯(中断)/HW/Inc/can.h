#ifndef __CAN_H
#define __CAN_H

#include "stm32f1xx_hal.h"

#define CAN1_TX_PORT      GPIOB
#define CAN1_TX_PIN       GPIO_PIN_9

#define CAN1_RX_PORT      GPIOB
#define CAN1_RX_PIN       GPIO_PIN_8

extern CAN_HandleTypeDef can1;				 // 控制结构体

void CAN1_Init(void);
void CAN_FilterInit(void);
void CAN_TxData(uint8_t *Tdata, uint8_t len);
uint8_t CAN_RxData(uint8_t *Rdata);

#endif /* __CAN_H */

