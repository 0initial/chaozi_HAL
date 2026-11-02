#include "spi.h"
#include "uart.h"
#include "w25q64.h"

SPI_HandleTypeDef spi1;
DMA_HandleTypeDef spi1_dmatx;
DMA_HandleTypeDef spi1_dmarx;

// SPI1接口初始化
void SPI1_Init(void)
{
	spi1.Instance = SPI1;                                  // 使用SPI1
	spi1.Init.Mode = SPI_MODE_MASTER;                      // 工作模式          SPI1
	spi1.Init.Direction = SPI_DIRECTION_2LINES;            // 双向模式          两线全双工
	spi1.Init.DataSize = SPI_DATASIZE_8BIT;                // 数据帧格式        按字节收发
	spi1.Init.CLKPolarity = SPI_POLARITY_LOW;              // 时钟极性          空闲时低电平
	spi1.Init.CLKPhase = SPI_PHASE_1EDGE;                  // 时钟相位          第一个边沿采集数据
	spi1.Init.NSS = SPI_NSS_SOFT;                          // 片选管理          软件模式
	spi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2; // 波特率分频控制     2分频 SPI1在APB2时钟线 72/2 -> 36M
	spi1.Init.FirstBit = SPI_FIRSTBIT_MSB;                 // 帧首发位          先发送高位
	spi1.Init.TIMode = SPI_TIMODE_DISABLE;                 // TI模式            不使用
	spi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE; // 硬件CRC校验       不使用
	spi1.Init.CRCPolynomial = 1;                           // 硬件CRC校验多项式  CRC校验没开启，这里不会生效
	HAL_SPI_Init(&spi1);
}

void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
	GPIO_InitTypeDef GPIO_InitTypeStruct;
	
	if(hspi->Instance == SPI1)
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_SPI1_CLK_ENABLE();
		__HAL_RCC_DMA1_CLK_ENABLE();
		
		// CS片选
		GPIO_InitTypeStruct.Pin = SPI1_CS_PIN;
		GPIO_InitTypeStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitTypeStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(SPI1_CS_PORT, &GPIO_InitTypeStruct);
		
		// CLK时钟线
		GPIO_InitTypeStruct.Pin = SPI1_CLK_PIN;
		GPIO_InitTypeStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitTypeStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(SPI1_CLK_PORT, &GPIO_InitTypeStruct);
		
		// 输入MISO
		GPIO_InitTypeStruct.Pin = SPI1_MISO_PIN;
		GPIO_InitTypeStruct.Mode = GPIO_MODE_AF_INPUT;
		GPIO_InitTypeStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(SPI1_MISO_PORT, &GPIO_InitTypeStruct);
		
		// 输出MOSI
		GPIO_InitTypeStruct.Pin = SPI1_MOSI_PIN;
		GPIO_InitTypeStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitTypeStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(SPI1_MOSI_PORT, &GPIO_InitTypeStruct);
		
		spi1_dmatx.Instance = DMA1_Channel3;
		spi1_dmatx.Init.Direction = DMA_MEMORY_TO_PERIPH;
		spi1_dmatx.Init.PeriphInc = DMA_PINC_DISABLE;
		spi1_dmatx.Init.MemInc = DMA_MINC_ENABLE;
		spi1_dmatx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		spi1_dmatx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
		spi1_dmatx.Init.Mode = DMA_NORMAL;
		spi1_dmatx.Init.Priority = DMA_PRIORITY_MEDIUM;
		__HAL_LINKDMA(hspi, hdmatx, spi1_dmatx);
		HAL_DMA_Init(&spi1_dmatx);
		
		spi1_dmarx.Instance = DMA1_Channel2;
		spi1_dmarx.Init.Direction = DMA_PERIPH_TO_MEMORY;
		spi1_dmarx.Init.PeriphInc = DMA_PINC_DISABLE;
		spi1_dmarx.Init.MemInc = DMA_MINC_ENABLE;
		spi1_dmarx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		spi1_dmarx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
		spi1_dmarx.Init.Mode = DMA_NORMAL;
		spi1_dmarx.Init.Priority = DMA_PRIORITY_MEDIUM;
		__HAL_LINKDMA(&spi1, hdmarx, spi1_dmarx);
		HAL_DMA_Init(&spi1_dmarx);
		
		// 开启中断
//		HAL_NVIC_SetPriority(SPI1_IRQn, 1, 0);
//		HAL_NVIC_EnableIRQ(SPI1_IRQn);
		
		HAL_NVIC_SetPriority(DMA1_Channel2_IRQn, 1, 0);
		HAL_NVIC_EnableIRQ(DMA1_Channel2_IRQn);
		
		HAL_NVIC_SetPriority(DMA1_Channel3_IRQn, 1, 0);
		HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);
	}
}

// SPI1接收中断回调函数
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
	if(hspi->Instance == SPI1)
	{
		W25Q64_CS_DISABLE();
		for(uint16_t i = 0; i < 4096; i++)
		{
			u1_printf("rbuff[%d] : %x\r\n", i, rbuff[i]);
		}
	}
}


