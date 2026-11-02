#include "spi.h"
#include "w25q64.h"
#include "uart.h"

// W25Q64等待空闲
void W25Q64_ReadBusy(void)
{
	uint8_t cmd[2], data[2];
	
	cmd[0] = W25X_READ_BUSY;
	cmd[1] = DUMMY_BYTE;
	
	do
	{
		W25Q64_CS_ENABLE();
		HAL_SPI_TransmitReceive(&spi1, cmd, data, 2, 1000);
		W25Q64_CS_DISABLE();
	}while((data[1] & 0x01) == 0x01);
}

// W25Q64写使能
void W25Q64_WriteEnable(void)
{
	uint8_t cmd;
	
	cmd = W25X_WRITE_ENABLE;
	
	W25Q64_ReadBusy();
	
	W25Q64_CS_ENABLE();
	HAL_SPI_Transmit(&spi1, &cmd, 1, 1000);
	W25Q64_CS_DISABLE();
}

// W25Q64扇区擦除(最小擦除单位 4KB)
void W25Q64_Sector_Erase(uint32_t SectorNum)
{
	uint8_t cmd[4];
	
	cmd[0] = W25X_SECTOR_ERASE;
	cmd[1] = (SectorNum * 4096) >> 16;
	cmd[2] = (SectorNum * 4096) >> 8;
	cmd[3] = SectorNum * 4096;
	
	W25Q64_WriteEnable();
	W25Q64_ReadBusy();
	
	W25Q64_CS_ENABLE();
	HAL_SPI_Transmit(&spi1, cmd, 4, 1000);
	W25Q64_CS_DISABLE();
}

// W25Q64块擦除(块擦除 64KB)
void W25Q64_Block_Erase(uint32_t BlockNum)
{
	uint8_t cmd[4];
	
	cmd[0] = W25X_BLOCK_ERASE;
	cmd[1] = (BlockNum * 65536) >> 16;
	cmd[2] = (BlockNum * 65536) >> 8;
	cmd[3] = BlockNum * 65536;
	
	W25Q64_WriteEnable();
	W25Q64_ReadBusy();
	
	W25Q64_CS_ENABLE();
	HAL_SPI_Transmit(&spi1, cmd, 4, 1000);
	W25Q64_CS_DISABLE();
}

// W25Q64写入一页数据(最小擦写入单位 256Byte)
void W25Q64_Page_Write(uint32_t PageNum, uint8_t *data)
{
	uint8_t cmd[4];
	
	cmd[0] = W25X_WRITE_PAGE;
	cmd[1] = (PageNum * 256) >> 16;
	cmd[2] = (PageNum * 256) >> 8;
	cmd[3] = PageNum * 256;
	
	W25Q64_WriteEnable();
	W25Q64_ReadBusy();
	
	W25Q64_CS_ENABLE();
	HAL_SPI_Transmit(&spi1, cmd, 4, 1000);
	HAL_SPI_Transmit(&spi1, data, 256, 5000);
	W25Q64_CS_DISABLE();
}



// W25Q64读取数据
void W25Q64_Read_Data(uint32_t addr, uint32_t len, uint8_t *rdata)
{
	uint8_t cmd[4];
	
	cmd[0] = W25X_READ_DATA;
	cmd[1] = addr >> 16;
	cmd[2] = addr >> 8;
	cmd[3] = addr;
	
	W25Q64_WriteEnable();
	W25Q64_ReadBusy();
	
	W25Q64_CS_ENABLE();
	HAL_SPI_Transmit(&spi1, cmd, 4, 1000);
	HAL_SPI_Receive_IT(&spi1, rdata, len);
}


