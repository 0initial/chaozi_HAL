#include "adc.h"
#include "uart.h"

ADC_HandleTypeDef           adc1;       // ADC1的总控结构体
ADC_ChannelConfTypeDef      adc1_in[3]; // ADC1的三个通道的结构体
DMA_HandleTypeDef           adc1_dma;   // ADC1的DMA总控结构体

uint16_t adc1_dmabuff[9];               // ADC1的DMA数组缓冲区

void ADC_Init(void)
{
	adc1.Instance = ADC1;
	adc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;        // 12位的ADC数据在16位的寄存器中右对齐
	adc1.Init.ScanConvMode = ADC_SCAN_ENABLE;         // 扫描模式
	adc1.Init.NbrOfConversion = 3;                    // 一次扫描3个通道
	adc1.Init.DiscontinuousConvMode = ENABLE;         // 禁用间断模式
	// 分组：通道11，通道1 / 通道2 最后一组，有几个算几个
	adc1.Init.NbrOfDiscConversion = 2;                // 规则组模式下，每次间断的数量。这里为2，表示2个通道为一个间断。
	adc1.Init.ContinuousConvMode = DISABLE;           // 连续转换模式
	adc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_EXT_IT11;  // 规则组外部触发选择
	HAL_ADC_Init(&adc1);                              // 配置ADC相关寄存器

	adc1_in[0].Channel = ADC_CHANNEL_11;                  // 通道11
	adc1_in[0].Rank = ADC_REGULAR_RANK_1;                 // 排名1
	adc1_in[0].SamplingTime = ADC_SAMPLETIME_41CYCLES_5;  // 选择41.5个周期，最终周期41.5 + 12.5。一个ADC采集时间为4.5us
	HAL_ADC_ConfigChannel(&adc1, &adc1_in[0]);            // 配置规则组通道

	adc1_in[1].Channel = ADC_CHANNEL_1;                   // 通道1
	adc1_in[1].Rank = ADC_REGULAR_RANK_2;                 // 排名2
	adc1_in[1].SamplingTime = ADC_SAMPLETIME_41CYCLES_5;  // 选择41.5个周期，最终周期41.5 + 12.5。一个ADC采集时间为4.5us
	HAL_ADC_ConfigChannel(&adc1, &adc1_in[1]);            // 配置规则组通道

	adc1_in[2].Channel = ADC_CHANNEL_2;                   // 通道2
	adc1_in[2].Rank = ADC_REGULAR_RANK_3;                 // 排名3
	adc1_in[2].SamplingTime = ADC_SAMPLETIME_41CYCLES_5;  // 选择41.5个周期，最终周期41.5 + 12.5。一个ADC采集时间为4.5us
	HAL_ADC_ConfigChannel(&adc1, &adc1_in[2]);            // 配置规则组通道

	HAL_ADCEx_Calibration_Start(&adc1);                     // 在开启前进行一次自校准
	HAL_ADC_Start_DMA(&adc1, (uint32_t *)adc1_dmabuff, 9);  // 开启转换，DMA方式，10次传输
}

void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
	GPIO_InitTypeDef GPIO_InitTypeStruct;

	if(hadc->Instance == ADC1)
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_GPIOC_CLK_ENABLE();
		__HAL_RCC_ADC1_CLK_ENABLE();
		__HAL_RCC_DMA1_CLK_ENABLE();

		// ADC1通道11 PC11
		GPIO_InitTypeStruct.Pin = ADC1_CHANNEL11_PIN;
		GPIO_InitTypeStruct.Mode = GPIO_MODE_ANALOG;
		HAL_GPIO_Init(ADC1_CHANNEL11_PORT, &GPIO_InitTypeStruct);

		// ADC1通道2 PA1
		GPIO_InitTypeStruct.Pin = ADC1_CHANNEL1_PIN;
		GPIO_InitTypeStruct.Mode = GPIO_MODE_ANALOG;
		HAL_GPIO_Init(ADC1_CHANNEL1_PORT, &GPIO_InitTypeStruct);

		// ADC1通道3 PA2
		GPIO_InitTypeStruct.Pin = ADC1_CHANNEL2_PIN;
		GPIO_InitTypeStruct.Mode = GPIO_MODE_ANALOG;
		HAL_GPIO_Init(ADC1_CHANNEL2_PORT, &GPIO_InitTypeStruct);

		// ADC1外部中断11 PA11
		GPIO_InitTypeStruct.Pin = ADC1_EXTI11_PIN;
		GPIO_InitTypeStruct.Mode = GPIO_MODE_IT_RISING;
		GPIO_InitTypeStruct.Pull = GPIO_PULLDOWN;
		HAL_GPIO_Init(ADC1_EXTI11_PORT, &GPIO_InitTypeStruct);
		
		adc1_dma.Instance = DMA1_Channel1;
		adc1_dma.Init.Direction = DMA_PERIPH_TO_MEMORY;
		adc1_dma.Init.PeriphInc = DMA_PINC_DISABLE;
		adc1_dma.Init.MemInc = DMA_MINC_ENABLE;
		adc1_dma.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
		adc1_dma.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
		adc1_dma.Init.Mode = DMA_CIRCULAR;
		adc1_dma.Init.Priority = DMA_PRIORITY_MEDIUM;
		__HAL_LINKDMA(hadc, DMA_Handle, adc1_dma);
		HAL_DMA_Init(&adc1_dma);

		HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 1, 0);
		HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
	}
}

// ADC的DMA完成中断回调
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	if(hadc->Instance == ADC1)
	{
		static float in11, in1, in2;
		
		in11 = 0;
		in1 = 0;
		in2 = 0;
		
		for(uint8_t i = 0; i < 3; i++)
		{
			in11 += adc1_dmabuff[i * 3];
			in1 += adc1_dmabuff[i * 3 + 1];
			in2 += adc1_dmabuff[i * 3 + 2];
		}
		u1_printf("ADC1_IN11 : %f\r\n", (in11 / 3) * (3.3 / 4096));
		u1_printf("ADC1_IN1  : %f\r\n", (in1 / 3) * (3.3 / 4096));
		u1_printf("ADC1_IN2  : %f\r\n", (in2 / 3) * (3.3 / 4096));
		
		HAL_ADC_Stop_DMA(&adc1);
	}
}


