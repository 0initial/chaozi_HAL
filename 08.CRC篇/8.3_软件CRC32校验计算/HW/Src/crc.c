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
	uint32_t poly = 0x04C11DB7;
	
	while(len--)
	{
		init ^= (*data << 24);
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
	
	return init;
}





