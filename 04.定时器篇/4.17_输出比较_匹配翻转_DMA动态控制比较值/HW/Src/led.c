#include "led.h"

void LED_Init(void)
{
	LED_GPIO_Init();
	LED_All_DISABLE();
}

void LED_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitTypeStruct;
	
	// 开启时钟
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	GPIO_InitTypeStruct.Pin = LED_RED_PIN | LED_GREEN_PIN | LED_BLUE_PIN;
	// 输出模式，弱上拉和下拉被禁止，手册有说明
	// GPIO_InitTypeStruct.Pull = ;
	GPIO_InitTypeStruct.Pull = GPIO_PULLDOWN;

	GPIO_InitTypeStruct.Mode = GPIO_MODE_INPUT;
	HAL_GPIO_Init(LED_RED_PORT, &GPIO_InitTypeStruct);

//	GPIO_InitTypeStruct.Mode = GPIO_MODE_OUTPUT_PP;
//	HAL_GPIO_Init(LED_GREEN_PORT, &GPIO_InitTypeStruct);

//	GPIO_InitTypeStruct.Mode = GPIO_MODE_OUTPUT_PP;
//	HAL_GPIO_Init(LED_BLUE_PORT, &GPIO_InitTypeStruct);
}

void LED_All_DISABLE(void)
{
	LED_RED_DISABLE();
	LED_GREEN_DISABLE();
	LED_BLUE_DISABLE();
}

void LED_Red(void)
{
	LED_RED_ENABLE();
	LED_GREEN_DISABLE();
	LED_BLUE_DISABLE();
}

void LED_Green(void)
{
	LED_RED_DISABLE();
	LED_GREEN_ENABLE();
	LED_BLUE_DISABLE();
}

void LED_Blue(void)
{
	LED_RED_DISABLE();
	LED_GREEN_DISABLE();
	LED_BLUE_ENABLE();
}

void LED_Yellow(void)
{
	LED_RED_ENABLE();
	LED_GREEN_ENABLE();
	LED_BLUE_DISABLE();
}

void LED_Purple(void)
{
	LED_RED_ENABLE();
	LED_GREEN_DISABLE();
	LED_BLUE_ENABLE();
}

void LED_Cyan(void)
{
	LED_RED_DISABLE();
	LED_GREEN_ENABLE();
	LED_BLUE_ENABLE();
}
