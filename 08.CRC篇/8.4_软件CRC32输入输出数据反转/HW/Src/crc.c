#include "crc.h"

CRC_HandleTypeDef crc;

// CRC初始化
void CRC_Init(void)
{
	crc.Instance = CRC;
	HAL_CRC_Init(&crc);
}

void HAL_CRC_MspInit(CRC_HandleTypeDef *hcrc)
{
	__HAL_RCC_CRC_CLK_ENABLE();
}

uint32_t CRC32(uint8_t *data, uint16_t len, uint32_t init)
{
	uint32_t poly = 0x12345678;
	
	while(len--)
	{
		init ^= (CRC32_InvertByte(*data) << 24);
		for(uint8_t i = 0; i < 8; i++)
		{
			if(init & 0x80000000) // 成立，该二进制位是1
			{
				init = (init << 1) ^ poly;
			}
			else                  // 不成立，该二进制位是0
			{
				init = (init << 1);
			}
		}
		data++;
	}
	
	return CRC32_InvertWord(init);
}

// 一字节反转
uint8_t CRC32_InvertByte(uint8_t data)
{
	uint8_t temp = 0;

	for(uint8_t i = 0; i < 8; i++)
	{
		if(data & (1 << i))
		{
			temp |= (1 << (7 - i));
		}
	}
	
	return temp;
}

// 四字节反转
uint32_t CRC32_InvertWord(uint32_t data)
{
	uint32_t temp = 0;

	for(uint8_t i = 0; i < 32; i++)
	{
		if(data & (1 << i))
		{
			temp |= (1 << (31 - i));
		}
	}
	
	return temp;
}

