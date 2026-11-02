#include "key.h"
#include "led.h"

uint8_t Key_Num = 0;

void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitTypeStruct;

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();

	GPIO_InitTypeStruct.Pin = KEY1_PIN;
	GPIO_InitTypeStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitTypeStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(KEY1_PROT, &GPIO_InitTypeStruct);
	
	GPIO_InitTypeStruct.Pin =  KEY2_PIN;
	GPIO_InitTypeStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitTypeStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(KEY2_PROT, &GPIO_InitTypeStruct);
}

/**
 * @brief  获取按下的按键
 * @param  无
 * @retval 无
 */
uint8_t Key_GetNum(void)
{
	uint8_t Temp;
	if(Key_Num)
	{
		Temp = Key_Num;
		Key_Num = 0;
		return Temp;
	}
	
	return 0;
}

/**
 * @brief  按键1扫描函数
 * @param  无
 * @retval 无
 */
uint8_t Key_Scan(void)
{
	if(KEY1_READ() == GPIO_PIN_SET)
	{
		return 1;
	}
	else if(KEY2_READ() == GPIO_PIN_SET)
	{
		return 2;
	}
	
	return 0;
}

/**
 * @brief  按键中断函数
 * @param  无
 * @retval 无
 */
void Key_Tick(void)
{
	static uint8_t CurrentState, PrevState;
	
	PrevState = CurrentState;
	CurrentState = Key_Scan();
	
	// 如果按键松开
	if(CurrentState == GPIO_PIN_RESET || PrevState != GPIO_PIN_RESET)
	{
		Key_Num = PrevState;
	}
}



