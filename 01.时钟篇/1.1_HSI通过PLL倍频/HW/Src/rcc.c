#include "stm32f1xx_hal.h"
#include "rcc.h"

void RccClock_Init(void)
{
	// 定义内外部时钟源配置结构体
	RCC_OscInitTypeDef RCC_OscInitTypeStruct;
	// 定义总线时钟配置结构体
	RCC_ClkInitTypeDef RCC_CLKInitTypeStruct;
	
	
	// 需要配置的震荡电路
	RCC_OscInitTypeStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	// 内部高速时钟状态
	RCC_OscInitTypeStruct.HSIState = RCC_HSI_ON;
	// HSI 微调值
	RCC_OscInitTypeStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	
	// PLL状态
	RCC_OscInitTypeStruct.PLL.PLLState = RCC_PLL_ON;
	// PLL输入源
	RCC_OscInitTypeStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
	// PLL倍频值
	RCC_OscInitTypeStruct.PLL.PLLMUL = RCC_PLL_MUL16;
	HAL_RCC_OscConfig(&RCC_OscInitTypeStruct);
	
	// 总线需要配置的时钟
	RCC_CLKInitTypeStruct.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	// 时钟来源
	RCC_CLKInitTypeStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_CLKInitTypeStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_CLKInitTypeStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_CLKInitTypeStruct.APB2CLKDivider = RCC_HCLK_DIV1;
	HAL_RCC_ClockConfig(&RCC_CLKInitTypeStruct, FLASH_LATENCY_2);
}
