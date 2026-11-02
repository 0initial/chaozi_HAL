#include "stm32f1xx_hal.h"

uint32_t HCLKFreq;
uint32_t PCLK1Freq;
uint32_t PCLK2Freq;

int main(void)
{
	HAL_Init();
	
	// 让 PA8 输出内部 HSI 8MHz时钟
	HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_SYSCLK, RCC_MCODIV_1);
	
	SystemCoreClockUpdate();
	// 利用变量计算输出的主频
	HCLKFreq = HAL_RCC_GetHCLKFreq();
	PCLK1Freq = HAL_RCC_GetPCLK1Freq();
	PCLK2Freq = HAL_RCC_GetPCLK2Freq();
	
	while(1)
	{
		
	}
}

