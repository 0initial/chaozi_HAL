#ifndef __CRC_H
#define __CRC_H

#include "stm32f1xx_hal.h"

extern CRC_HandleTypeDef crc;

void CRC_Init(void);
uint32_t CRC32(uint8_t *data, uint16_t len, uint32_t init);
uint16_t CRC16(uint8_t *data, uint16_t len, uint16_t init);
uint8_t CRC8(uint8_t *data, uint16_t len, uint8_t init);

uint8_t CRC32_InvertByte(uint8_t data);
uint16_t CRC32_InvertHalfWrod(uint16_t data);
uint32_t CRC32_InvertWord(uint32_t data);


#endif
