#ifndef __W25Q64_H
#define __W25Q64_H

#include "stm32f1xx_hal.h"

#define W25Q64_CS_ENABLE()     HAL_GPIO_WritePin(SPI1_CS_PORT, SPI1_CS_PIN, GPIO_PIN_RESET)
#define W25Q64_CS_DISABLE()    HAL_GPIO_WritePin(SPI1_CS_PORT, SPI1_CS_PIN, GPIO_PIN_SET)

#define W25X_WRITE_PAGE        0x02
#define W25X_READ_DATA         0x03
#define W25X_READ_BUSY         0x05
#define W25X_WRITE_ENABLE      0x06
#define W25X_SECTOR_ERASE      0x20
#define W25X_BLOCK_ERASE       0xD8

#define DUMMY_BYTE             0xFF

void W25Q64_ReadBusy(void);
void W25Q64_WriteEnable(void);
void W25Q64_Sector_Erase(uint32_t SectorNum);
void W25Q64_Block_Erase(uint32_t BlockNum);
void W25Q64_Page_Write(uint32_t PageNum, uint8_t *data);
void W25Q64_Read_Data(uint32_t addr, uint32_t len, uint8_t *rdata);

#endif
