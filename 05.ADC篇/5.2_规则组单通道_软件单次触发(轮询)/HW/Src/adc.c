#include "adc.h"
#include "uart.h"

ADC_HandleTypeDef           adc1;     // ADC1的总控结构体
ADC_ChannelConfTypeDef      adc1_in0; // ADC1的通道0结构体

void ADC_Init(void)
{
	adc1.Instance = ADC1;
	adc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;        // 12位的ADC数据在16位的寄存器中右对齐
	adc1.Init.ScanConvMode = ADC_SCAN_DISABLE;        // 扫描模式。关闭
	adc1.Init.ContinuousConvMode = DISABLE;           // 连续转换模式。关闭
	adc1.Init.NbrOfConversion = 1;                    // 扫描模式下，规则组通道的数量。必须使能扫描模式,例程没有开启扫描模式，所以写了也无效，仅保证模板清晰
	adc1.Init.DiscontinuousConvMode = DISABLE;        // 规则组间断模式。必须使能扫描模式，例程没有开启扫描模式，所以写了也无效，仅保证模板清晰
	adc1.Init.NbrOfDiscConversion = 1;                // 规则组间断模式下，每次间断的通道数。必须使能规则组间断模式，历程中没有开启扫描模式，所以间断模式也没有开启，那么本成员也无效，仅保证模板清晰
	adc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;  // 规则组外部触发选择。本次选择软件触发
	HAL_ADC_Init(&adc1);                              // 配置ADC相关寄存器
	
	adc1_in0.Channel = ADC_CHANNEL_0;                  // 通道1
	adc1_in0.Rank = ADC_REGULAR_RANK_1;                // 排名1
	adc1_in0.SamplingTime = ADC_SAMPLETIME_41CYCLES_5; // 选择41.5个周期，最终周期41.5 + 12.5。一个ADC采集时间为4.5us
	HAL_ADC_ConfigChannel(&adc1, &adc1_in0);           // 配置规则组通道
	
	HAL_ADCEx_Calibration_Start(&adc1);  // 在开启前进行一次自校准
	HAL_ADC_Start(&adc1);                // 开启转换，轮询模式
}

void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
	GPIO_InitTypeDef GPIO_InitTypeStruct;
	
	if(hadc->Instance == ADC1)
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_GPIOC_CLK_ENABLE();
		__HAL_RCC_ADC1_CLK_ENABLE();
		
		GPIO_InitTypeStruct.Pin = GPIO_PIN_1;
		GPIO_InitTypeStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitTypeStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(GPIOC, &GPIO_InitTypeStruct);
		
		GPIO_InitTypeStruct.Pin = ADC1_CHANNEL1_PIN;
		GPIO_InitTypeStruct.Mode = GPIO_MODE_ANALOG;
		HAL_GPIO_Init(ADC1_CHANNEL1_PORT, &GPIO_InitTypeStruct);
	}
}




