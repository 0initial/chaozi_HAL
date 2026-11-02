#include "crc.h"

CRC_HandleTypeDef crc;

// CRC≥ı ºªØ
void CRC_Init(void)
{
	crc.Instance = CRC;
	HAL_CRC_Init(&crc);
}

void HAL_CRC_MspInit(CRC_HandleTypeDef *hcrc)
{
	__HAL_RCC_CRC_CLK_ENABLE();
}



