#include "adc.h"
#include "uart.h"

ADC_HandleTypeDef           adc1;      // ADC1的总控结构体
ADC_ChannelConfTypeDef      adc1_in11; // ADC1的通道11结构体

void ADC_Init(void)
{
	adc1.Instance = ADC1;
	adc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;        // 12位的ADC数据在16位的寄存器中右对齐
	adc1.Init.ScanConvMode = ADC_SCAN_DISABLE;        // 扫描模式
	adc1.Init.ContinuousConvMode = ENABLE;            // 连续转换模式。
	adc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;  // 规则组外部触发选择
	HAL_ADC_Init(&adc1);                              // 配置ADC相关寄存器
	
	adc1_in11.Channel = ADC_CHANNEL_11;                  // 通道11
	adc1_in11.Rank = ADC_REGULAR_RANK_1;                 // 排名1
	adc1_in11.SamplingTime = ADC_SAMPLETIME_41CYCLES_5;  // 选择41.5个周期，最终周期41.5 + 12.5。一个ADC采集时间为4.5us
	HAL_ADC_ConfigChannel(&adc1, &adc1_in11);            // 配置规则组通道
	
	HAL_ADCEx_Calibration_Start(&adc1);  // 在开启前进行一次自校准
	HAL_ADC_Start(&adc1);                // 开启转换，轮询模式
}

void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
	GPIO_InitTypeDef GPIO_InitTypeStruct;
	
	if(hadc->Instance == ADC1)
	{
		__HAL_RCC_GPIOC_CLK_ENABLE();
		__HAL_RCC_ADC1_CLK_ENABLE();
		
		GPIO_InitTypeStruct.Pin = ADC1_CHANNEL11_PIN;
		GPIO_InitTypeStruct.Mode = GPIO_MODE_ANALOG;
		HAL_GPIO_Init(ADC1_CHANNEL11_PORT, &GPIO_InitTypeStruct);
	}
}




